#include <glad/glad.h>

#include <string>
#include <fstream>

#include "Core/Base.h"
#include "Shader.h"

namespace Logl
{
	static std::string ShaderTypeToString(GLenum type)
	{
		if (type == GL_VERTEX_SHADER)
			return "VERTEX";
		if (type == GL_FRAGMENT_SHADER)
			return "FRAGMENT";
		if (type == GL_GEOMETRY_SHADER)
			return "GEOMETRY";

		return "Unknown shader type!";
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;

		PRINT("Unknown shader type!");
		__debugbreak();
		return 0;
	}

	static unsigned int CompileShader(GLenum type, const char* sourceCode)
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &sourceCode, nullptr);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			PRINT("Faild to compile {0} shader!\n{1}\n", ShaderTypeToString(type), infoLog);
			glDeleteShader(shader);
			return -1;
		}

		return shader;
	}

	static bool CheckProgramLinkError(unsigned int programId)
	{
		int success{};
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			int length;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			PRINT("Failed to link shader program!\n{}\n", infoLog);
			return false;
		}

		return true;
	}


	static std::string ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
			}
			else
			{
				PRINT("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			PRINT("Could not open file {}\n", filepath);
		}

		return result;
	}


	Shader::Shader(const std::string& filePath)
		: m_Id(0)
	{
		std::string str = ReadFile(filePath);
		auto sourceCodes = PreProcess(str);
		Compile(sourceCodes);
	}

	Shader::Shader(const std::string& vsPath, const std::string& fsPath) 
		: m_Id(0)
	{
		auto vsStr = ReadFile(vsPath);
		auto fsStr = ReadFile(fsPath);

		const char* vsCode = vsStr.c_str();
		const char* fsCode = fsStr.c_str();
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vsCode);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (!CheckProgramLinkError(shaderProgram))
			return;

		m_Id = shaderProgram;
	}

	Shader::Shader(const char* vsCode, const char* fsCode, bool bUse)
		: m_Id(0)
	{
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vsCode);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (!CheckProgramLinkError(shaderProgram))
		{
			glDeleteProgram(shaderProgram);
			return;
		}

		m_Id = shaderProgram;

		if(bUse)
			glUseProgram(m_Id);
	}

	Shader::~Shader()
	{
		glDeleteShader(m_Id);
	}

	void Shader::Use() const
	{
		glUseProgram(m_Id); 
	}

	void Shader::SetInt(const std::string& name, int value) 
	{
		glUniform1i(Location(name), value); 
	}

	void Shader::SetFloat(const std::string& name, float value) 
	{ 
		glUniform1f(Location(name), value); 
	}

	void Shader::SetFloat3(const std::string& name, const vec3& vec)
	{
		glUniform3f(Location(name), vec.x, vec.y, vec.z);
	}

	void Shader::SetFloat4(const std::string& name, const vec4& vec)
	{
		glUniform4f(Location(name), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::SetMat4(const std::string& name, const mat4& mat)
	{ 
		glUniformMatrix4fv(Location(name), 1, GL_FALSE, mat.ValuePtr()); 
	}


	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			ASSERT(eol != std::string::npos); // Syntax error
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			ASSERT(ShaderTypeFromString(type)); // Invalid shader type specified

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			ASSERT(nextLinePos != std::string::npos);  // Syntax error
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? 
				source.substr(nextLinePos) : 
				source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		auto size = shaderSources.size();
		ASSERT(size > 0 && size <= 3);
		std::vector<GLuint> glShaderIDs;
		glShaderIDs.reserve(3);

		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			const GLchar* sourceCStr = source.c_str();
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				PRINT("Faild to compile {0} shader!\n{1}\n", ShaderTypeToString(type), infoLog.data());
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_Id = program;

		// Link
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength); // The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto id : glShaderIDs)
				glDeleteShader(id);
			PRINT("Failed to link shader program!\n{}\n", infoLog.data());
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	int Shader::Location(const std::string& name)
	{
		auto it = m_uniforms.find(name);
		if (it == m_uniforms.end())
		{
			m_uniforms[name] = glGetUniformLocation(m_Id, name.c_str());
		}

		if (m_uniforms[name] == -1)
		{
			PRINT("Invalid uniform: {}\n", name);
			//__debugbreak();
		}
		return m_uniforms[name];
	}
}
