#include <glad/glad.h>

#include <string>
#include <fstream>

#include "Core/Base.h"
#include "Shader.h"

namespace Logl
{

	static std::unordered_map<GLenum, std::string> s_ShaderName =
	{
		{ GL_VERTEX_SHADER, "VERTEX" },
		{ GL_FRAGMENT_SHADER, "FRAGMENT" },
		{ GL_GEOMETRY_SHADER, "GEOMETRY" },
	};

	static std::string ReadFileAsString(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize((size_t)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			PRINT("Could not open file {}\n", filepath);
		}

		return result;
	}

	static unsigned int CompileShader(unsigned int type, const char* sourceCode)
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
			PRINT("Faild to compile {0} shader!\n{1}\n", s_ShaderName[type], infoLog);
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

	Shader::Shader(const std::string& vsPath, const std::string& fsPath) : m_Id(0)
	{
		auto vsStr = ReadFileAsString(vsPath);
		auto fsStr = ReadFileAsString(fsPath);

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

	Shader::Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath) : m_Id(0)
	{
		auto vsStr = ReadFileAsString(vsPath);
		auto fsStr = ReadFileAsString(fsPath);
		auto gsStr = ReadFileAsString(gsPath);

		const char* vsCode = vsStr.c_str();
		const char* fsCode = fsStr.c_str();
		const char* gsCode = gsStr.c_str();

		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vsCode);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
		unsigned int geometryShader = CompileShader(GL_GEOMETRY_SHADER, gsCode);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, geometryShader);
		glLinkProgram(shaderProgram);

		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);
		glDetachShader(shaderProgram, geometryShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

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
			return;

		m_Id = shaderProgram;

		if(bUse)
			glUseProgram(m_Id);
	}

	Shader::~Shader()
	{
		glDeleteShader(m_Id);
	}

	bool Shader::IsValid() const
	{
		return m_Id > 0; 
	}

	void Shader::Use() const
	{
		glUseProgram(m_Id); 
	}

	void Shader::SetUniform(const std::string& name, float value) 
	{ 
		glUniform1f(Location(name), value); 
	}

	void Shader::SetUniform(const std::string& name, int value) 
	{
		glUniform1i(Location(name), value); 
	}

	void Shader::SetUniform(const std::string& name, const float* value) 
	{ 
		glUniformMatrix4fv(Location(name), 1, GL_FALSE, value); 
	}

	void Shader::SetUniform3f(const std::string& name, float x, float y, float z)
	{
		glUniform3f(Location(name), x, y, z);
	}

	void Shader::SetUniform(const std::string& name, vec3 vec)
	{
		glUniform3f(Location(name), vec.x, vec.y, vec.z);
	}

	void Shader::SetUniform(const std::string& name, vec4 vec)
	{
		glUniform4f(Location(name), vec.x, vec.y, vec.z, vec.w);
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
