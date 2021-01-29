#pragma once

#include <unordered_map>

#include "FMT.h"

namespace Logl
{
	class Shader
	{
	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
		~Shader();

		bool IsValid() { return m_Id > 0; }

		void Use() { glUseProgram(m_Id); }
		void SetUniform(const std::string& name, float value) { glUniform1f(Location(name), value); }
		void SetUniform(const std::string& name, int value) { glUniform1i(Location(name), value); }
		void SetUniform(const std::string& name, const float* value) { glUniformMatrix4fv(Location(name), 1, GL_FALSE, value); }

	private:
		int Location(const std::string& name)
		{
			auto it = m_uniforms.find(name);
			if (it == m_uniforms.end())
			{
				m_uniforms[name] = glGetUniformLocation(m_Id, name.c_str());
			}

			if (m_uniforms[name] == -1)
			{
				PRINT("Invalid uniform: {}\n", name);
				__debugbreak();
			}
			return m_uniforms[name];
		}

	private:
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_uniforms;
	};
}