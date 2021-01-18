#include <glad/glad.h>

#include <string>
#include <fstream>

#include "FMT.h"
#include "Shader.h"

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
		PRINT("Failed to link shader program!\n{1}\n", infoLog);
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

Shader::~Shader()
{
	glDeleteShader(m_Id);
}

bool Shader::IsValid()
{
	return m_Id > 0;
}

void Shader::Use()
{
	glUseProgram(m_Id);
}

void Shader::SetUniform(const std::string& name, float value)
{
	auto it = m_uniforms.find(name);
	if (it == m_uniforms.end())
	{
		m_uniforms[name] = glGetUniformLocation(m_Id, name.c_str());
	}

	int location = m_uniforms[name];
	if (location == -1)
	{
		PRINT("Invalid uniform: {}\n", name);
		__debugbreak();
		return;
	}
	
	glUniform1f(m_uniforms[name], value);
}


