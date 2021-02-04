#pragma once

#include <unordered_map>

namespace Logl
{
	class Shader
	{
	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
		~Shader();

		bool IsValid() const;

		void Use() const;
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, const float* value);

	private:
		int Location(const std::string& name);

	private:
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_uniforms;
	};
}