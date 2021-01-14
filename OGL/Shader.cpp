#include <glad/glad.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "Shader.h"

static std::map<GLenum, std::string> s_ShaderName =
{
	{GL_VERTEX_SHADER, "VertexShader"},
	{GL_FRAGMENT_SHADER, "FragmentShader"},
	{GL_GEOMETRY_SHADER, "GeometryShader"},
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
		std::cout << "Could not open file " << filepath << std::endl;
	}

	return result;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath) : m_Id(0)
{
	auto vsStr = ReadFileAsString(vsPath);
	auto fsStr = ReadFileAsString(fsPath);

	const char* vsCode = vsStr.c_str();
	const char* fsCode = fsStr.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsCode, nullptr);
	glCompileShader(vertexShader);
	if (!CheckShaderCompileError(vertexShader, GL_VERTEX_SHADER))
		return;

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fsCode, nullptr);
	glCompileShader(fragmentShader);
	if (!CheckShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER))
		return;

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	m_Id = shaderProgram;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath) : m_Id(0)
{
	auto vsStr = ReadFileAsString(vsPath);
	auto fsStr = ReadFileAsString(vsPath);
	auto gsStr = ReadFileAsString(gsPath);

	const char* vsCode = vsStr.c_str();
	const char* fsCode = fsStr.c_str();
	const char* gsCode = gsStr.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsCode, nullptr);
	glCompileShader(vertexShader);
	if (!CheckShaderCompileError(vertexShader, GL_VERTEX_SHADER))
		return;

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fsCode, nullptr);
	glCompileShader(fragmentShader);
	if (!CheckShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER))
		return;

	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &fsCode, nullptr);
	glCompileShader(geometryShader);
	if (!CheckShaderCompileError(geometryShader, GL_GEOMETRY_SHADER))
		return;

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);

	m_Id = shaderProgram;
}

Shader::~Shader()
{
	glDeleteShader(m_Id);
}

bool Shader::CheckShaderCompileError(unsigned int shaderId, GLenum type)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << s_ShaderName[type] << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

bool Shader::IsValid()
{
	return m_Id;
}

void Shader::Use()
{
	glUseProgram(m_Id);
}


