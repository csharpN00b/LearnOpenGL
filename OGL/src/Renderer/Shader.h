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
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat3(const std::string& name, const vec3& vec);
		void SetFloat4(const std::string& name, const vec4& vec);
		void SetMat4(const std::string& name, const mat4& mat);

	private:
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		int Location(const std::string& name);

	private:
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_uniforms;
	};
}