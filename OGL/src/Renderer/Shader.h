#pragma once

#include "Math/matrix4f.h"

#include <unordered_map>

namespace Logl
{
	class Shader
	{
	public:
		Shader(const std::string& filePath);
		Shader(const std::string& vsPath, const std::string& fsPath);
		Shader(const char* vsCode, const char* fsCode, bool bUse);
		Shader(const Shader&) = delete;
		~Shader();

		Shader& operator=(const Shader&) = delete;

		void Use() const;
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, const mat4& mat);
		void SetUniform3f(const std::string& name, float x, float y, float z);
		void SetUniform(const std::string& name, const vec3& vec);
		void SetUniform(const std::string& name, const vec4& vec);

	private:
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		int Location(const std::string& name);

	private:
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_uniforms;
	};
}