#pragma once

class Shader
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
	~Shader();

	bool IsValid();

	void Use();
	void SetUniform(const std::string& name, float value);
private:
	bool CheckShaderCompileError(unsigned int shaderId, GLenum type);

private:
	unsigned int m_Id;
	std::unordered_map<std::string, int> m_uniforms;
};