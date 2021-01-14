#pragma once

class Shader
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
	~Shader();

	bool IsValid();

	void Use();
private:
	bool CheckShaderCompileError(unsigned int shaderId, GLenum type);

private:
	unsigned int m_Id;
};