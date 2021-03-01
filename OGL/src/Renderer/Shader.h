#pragma once

#include "Math/Vector4f.h"

#include <unordered_map>

namespace Logl
{
	class Shader
	{
	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
		Shader(const Shader&) = delete;
		~Shader();

		Shader& operator=(const Shader&) = delete;

		bool IsValid() const;

		void Use() const;
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, const float* value);
		void SetUniform3f(const std::string& name, float x, float y, float z);
		void SetUniform(const std::string& name, vec3 vec);
		void SetUniform(const std::string& name, vec4 vec);

	private:
		int Location(const std::string& name);

	private:
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_uniforms;
	};
}