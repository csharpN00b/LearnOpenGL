#pragma once

#include <vector>

#include <Math/Matrix4f.h>
#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <Renderer/VertexArray.h>

namespace Logl
{
	typedef void(*DynamicUniform)(Shader* shader, float time, Camera* camera);

	struct TransparentModel
	{
		VertexArray* vao;
		Shader* shader;
		DynamicUniform dynamicUniform;
		mat4* transform;
		vec4* color;
		std::vector <Texture2D*>& textures;

		TransparentModel(VertexArray* vao, Logl::Shader* shader, Logl::DynamicUniform dynamicUniform, mat4* transform, vec4* color, std::vector<Texture2D*>& textures)
			: vao(vao), shader(shader), dynamicUniform(dynamicUniform), transform(transform), color(color), textures(textures)
		{}

		TransparentModel& operator=(const TransparentModel& model)
		{
			vao = model.vao;
			shader = model.shader;
			dynamicUniform = model.dynamicUniform;
			transform = model.transform;
			color = model.color;
			textures = model.textures;

			return *this;
		}
	};

	struct RenderObject
	{
		bool bTransparent;
		VertexArray* vao;
		Shader* shader;
		DynamicUniform dynamicUniform;
		std::vector<mat4> models;
		std::vector<vec4> colors;

		std::vector<Texture2D*> textures;

		RenderObject(VertexArray& vao, Shader& shader, DynamicUniform func, bool transparent = false)
			: vao(&vao), shader(&shader), dynamicUniform(func), bTransparent(transparent)
		{
		}

		RenderObject(VertexArray& vao, Shader& shader, mat4 model, bool transparent = false)
			: vao(&vao), shader(&shader), dynamicUniform(nullptr), bTransparent(false)
		{
			models.emplace_back(model);
		}

		void AddModel(const mat4& mat) { models.emplace_back(mat); }

		void AddModel(const mat4& mat, const vec3& color)
		{
			models.emplace_back(mat);
			colors.emplace_back(color);
		}

		void AddModel(const mat4& mat, const vec4& color)
		{
			bTransparent = true;
			models.emplace_back(mat);
			colors.emplace_back(color);
		}

		void AddTexture(Texture2D* texture) { textures.emplace_back(texture); }

		void GetTransparentModels(std::vector<TransparentModel>& transparentModels)
		{
			if(bTransparent)
			{
				for (int i = 0; i < models.size(); i++)
				{
					vec4* color = i < colors.size() ? &colors[i] : nullptr;
					transparentModels.emplace_back(vao, shader, dynamicUniform, &models[i], color, textures);
				}
			}
		}
	};

	
}