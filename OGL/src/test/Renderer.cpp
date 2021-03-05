#include "Renderer.h"

#include "Core/Base.h"

#include "Renderer/Shader.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/FrameBuffer.h"

#include <algorithm>

#define BLUR 1

namespace Logl
{
#define BIND_FUNC(name) std::bind(&Renderer::name, this, std::placeholders::_1)


	Renderer::Renderer(Window* window, bool bOrtho)
		: m_Window(window), m_EnableDepthTesting(false), m_EnableBlending(false), m_Running(false)
	{
		m_Window->SetEventCallback(BIND_FUNC(OnEvent));

		auto width = window->GetWidth();
		auto height = window->GetHeight();

		if (bOrtho)
		{
			float x = width * 0.01f * 0.5f;;
			float y = height * 0.01f * 0.5f;
			auto frustum = Frustum(-x, x, -y, y, 0.1f, 50.0f);
			auto position = vec3(0.0f, 0.0f, 20.0f);
#if 1
			m_Camera = new OrthographicCamera(frustum, position);
			m_Camera->SetViewport(0, 0, width, height);
#else
			m_Camera = new OrthoCamera(frustum, position);
#endif
			m_Camera->SetRotateSensitivity(0.1f);
		}
		else
		{
			float ratio = (float)width / (float)height;
			auto frustum = Frustum(ratio, 45.0f, 0.1f, 100.0f);
			auto position = vec3(0.0f, 0.0f, 5.0f);
			m_Camera = new PerspectiveCamera(frustum, position);

			m_Window->SetCursorPos(m_Window->GetWidth() * 0.5, m_Window->GetHeight() * 0.5);
			window->OnUpdate();
		}

		m_State.lastX = width / 2.0f;
		m_State.lastY = height / 2.0f;
	}

	Renderer::~Renderer()
	{
		delete m_Camera;
	}

	void Renderer::EnableDepthTest(GLenum func)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(func);

		m_EnableDepthTesting = true;
	}

	void Renderer::EnableBlend(GLenum sfactor, GLenum dfactor)
	{
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);

		m_EnableBlending = true;
	}

	void Renderer::SetCameraPos(vec3 pos)
	{
		m_Camera->SetPosition(pos);
	}

	vec3 Renderer::GetCameraPos() const
	{
		return m_Camera->GetPosition();
	}

	void Renderer::AddObject(RenderObject& obj)
	{
		if (m_EnableBlending)
		{
			if (obj.bTransparent)
			{
				obj.GetTransparentModels(m_TransparentList);
				return;
			}
		}

		m_Objects.push_back(&obj);
	}


	void Renderer::Render(vec3 backgroudColor)
	{
		m_Running = true;

		typedef void(*DrawCallFunc)(int);
		DrawCallFunc drawcall;
		auto drawElements = [](int count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); };
		auto drawArrays = drawcall = [](int count) { glDrawArrays(GL_TRIANGLES, 0, count); };

		if (m_Objects.size() == 0 && m_TransparentList.size() == 0)
			PRINT("no objects!");

		unsigned int mask = GL_COLOR_BUFFER_BIT;
		if (m_EnableDepthTesting)
			mask |= GL_DEPTH_BUFFER_BIT;

		// Loop
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_State.UpdateTime(time);

			processInput();

			glClearColor(backgroudColor.r, backgroudColor.g, backgroudColor.b, 1.0f);
			glClear(mask);

			auto projection = m_Camera->GetProjectionMatrix();
			auto view = m_Camera->GetViewMatrix();

			// non-transparent objects
			for (auto& obj : m_Objects)
			{
				drawcall = obj->vao->IsUsingIndex() ? drawElements : drawArrays;

				for (int i = 0; i < obj->textures.size(); i++)
					obj->textures[i]->Bind(i);

				obj->shader->Use();
				obj->shader->SetUniform("projection", projection);
				obj->shader->SetUniform("view", view);

				if (obj->dynamicUniform)
					obj->dynamicUniform(obj->shader, time, m_Camera);

				obj->vao->Bind();
				if (obj->models.size())
				{
					for (int i = 0; i < obj->models.size(); i++)
					{
						obj->shader->SetUniform("model", obj->models[i]);
						if (i < obj->colors.size())
						{
							if (obj->bTransparent)
								obj->shader->SetUniform("color", obj->colors[i]);
							else
								obj->shader->SetUniform3f("color", obj->colors[i].r, obj->colors[i].g, obj->colors[i].b);
						}

						drawcall(obj->vao->GetCount());
					}
				}
				else
					drawcall(obj->vao->GetCount());
			}

			// transparent models
			if (m_EnableBlending && m_TransparentList.size())
			{
				auto cameraPos = m_Camera->GetPosition();
				std::sort(m_TransparentList.begin(), m_TransparentList.end(), [&cameraPos](TransparentModel& t1, TransparentModel& t2)
					{
						auto d1 = distance((*t1.transform) * vec3(), cameraPos);
						auto d2 = distance((*t2.transform) * vec3(), cameraPos);
						return d1 > d2;
					});

				for (auto& obj : m_TransparentList)
				{
					drawcall = obj.vao->IsUsingIndex() ? drawElements : drawArrays;

					for (int i = 0; i < obj.textures.size(); i++)
						obj.textures[i]->Bind(i);

					obj.shader->Use();
					obj.shader->SetUniform("projection", projection);
					obj.shader->SetUniform("view", view);
					obj.shader->SetUniform("model", *obj.transform);

					if (obj.dynamicUniform)
						obj.dynamicUniform(obj.shader, time, m_Camera);

					if (obj.color)
						obj.shader->SetUniform("color", *(obj.color));

					obj.vao->Bind();
					drawcall(obj.vao->GetCount());
				}
			}


			m_Window->OnUpdate();
		}
	}

	void Renderer::RenderScreen(vec3 backgroudColor)
	{
		m_Running = true;

		typedef void(*DrawCallFunc)(int);
		DrawCallFunc drawcall;
		auto drawElements = [](int count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); };
		auto drawArrays = drawcall = [](int count) { glDrawArrays(GL_TRIANGLES, 0, count); };

		if (m_Objects.size() == 0)
			PRINT("no objects!");

		// another FrameBuffer
		FrameBuffer fbo;
		fbo.Bind();
		fbo.InitDefaultAttachment((int)m_Window->GetWidth(), (int)m_Window->GetHeight());
		if (!fbo.IsComplete())
		{
			PRINT("Framebuffer is not complete!");
			return;
		}
		fbo.Unbind();

		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		VertexArray quadVao;
		VertexBuffer quadVbo(quadVertices, sizeof(quadVertices), { {GL_FLOAT, 2}, {GL_FLOAT, 2} });
		quadVao.AddVertexBuffer(quadVbo);

#if BLUR
		Shader screenShader("asserts/shaders/framebuffers_screen_vs.glsl", "asserts/shaders/kernel_effects_blur_fs.glsl");
#else
		Shader screenShader("asserts/shaders/framebuffers_screen_vs.glsl", "asserts/shaders/framebuffers_screen_fs.glsl");
#endif
		screenShader.Use();
		screenShader.SetUniform("screenTexture", 0);

		// draw as wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		PRINT("render loop begin!\n");
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_State.UpdateTime(time);

			processInput();

			// bind to framebuffer and draw scene
			fbo.Bind();
			glEnable(GL_DEPTH_TEST);
			glClearColor(backgroudColor.r, backgroudColor.g, backgroudColor.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto projection = m_Camera->GetProjectionMatrix();
			auto view = m_Camera->GetViewMatrix();
			for (auto& obj : m_Objects)
			{
				auto drawcall = obj->vao->IsUsingIndex() ? drawElements : drawArrays;

				obj->shader->Use();
				obj->shader->SetUniform("projection", projection);
				obj->shader->SetUniform("view", view);

				for(auto& texture : obj->textures)
					texture->Bind(0);

				obj->vao->Bind();
				if (obj->models.size())
				{
					for (auto& m : obj->models)
					{
						obj->shader->SetUniform("model", m);
						drawcall(obj->vao->GetCount());
					}
				}
				else
					drawcall(obj->vao->GetCount());
			}

			// bind back to default framebuffer
			fbo.Unbind();
			glDisable(GL_DEPTH_TEST);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			screenShader.Use();
			
			quadVao.Bind();
			glBindTexture(GL_TEXTURE_2D, fbo.GetDefalutColorAttachmentId());
			drawArrays(quadVao.GetCount());

			m_Window->OnUpdate();
		}
		PRINT("render loop end!\n");
	}

	void Renderer::processInput()
	{
		//return;
		auto window = m_Window->GetNativeWindow();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->Move(MoveDirection::FORWARD, m_State.deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->Move(MoveDirection::BACKWARD, m_State.deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->Move(MoveDirection::LEFT, m_State.deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->Move(MoveDirection::RIGHT, m_State.deltaTime);
	}


	void Renderer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowClose)
		{
			m_Running = false;
			return;
		}

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_FUNC(OnMouseMove));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_FUNC(OnMouseScroll));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_FUNC(OnKeyPress));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_FUNC(OnKeyRelease));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_FUNC(OnMouseButtonPress));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_FUNC(OnMouseButtonRelease));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_FUNC(OnWindowResize));
	}

	bool Renderer::OnMouseMove(MouseMovedEvent event)
	{
		auto xold = m_State.lastX;
		auto yold = m_Window->GetHeight() - m_State.lastY; // window coordinates -> viewport coordinates
		auto pair = m_State.UpdatePos(event.GetXPos(), event.GetYPos());
		m_Camera->Turn(pair.first, pair.second);
		m_Camera->Move(pair.first, pair.second);

		auto xnew = m_State.lastX;;
		auto ynew = m_Window->GetHeight() - m_State.lastY; // window coordinates -> viewport coordinates
		m_Camera->Move(xold, yold, xnew, ynew);

		return true;
	}

	bool Renderer::OnMouseScroll(MouseScrolledEvent event)
	{
		m_Camera->Scale(event.GetYOffset());

		auto xpos = m_State.lastX;
		auto ypos = (float)m_Window->GetHeight() - m_State.lastY; // window coordinates -> viewport coordinates
		m_Camera->Scale(event.GetYOffset(), xpos, ypos);

		m_Camera->Scale(event.GetYOffset(), m_State.lastX, m_State.lastY, (float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		return true;
	}

	bool Renderer::OnKeyPress(KeyPressedEvent event)
	{
		auto key = event.GetKey();

		if (key == GLFW_KEY_ESCAPE)
			m_Running = false;
		else if(key == GLFW_KEY_F2)
			m_Camera->Reset();
		
#if 0
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
		{
			m_Running = false;
			break;
		}
		case GLFW_KEY_F2:
		{
			m_Camera->Reset();
			break;
		}
		case GLFW_KEY_W:
		{
			m_Camera->Move(MoveDirection::FORWARD, m_State.deltaTime);
			break;
		}
		case GLFW_KEY_S:
		{
			m_Camera->Move(MoveDirection::BACKWARD, m_State.deltaTime);
			break;
		}
		case GLFW_KEY_A:
		{
			m_Camera->Move(MoveDirection::LEFT, m_State.deltaTime);
			break;
		}
		case GLFW_KEY_D:
		{
			m_Camera->Move(MoveDirection::RIGHT, m_State.deltaTime);
			break;
		}
		default:
			break;
		}
#endif
		return true;
	}

	bool Renderer::OnKeyRelease(KeyReleasedEvent event)
	{
		return true;
	}

	bool Renderer::OnMouseButtonPress(MouseButtonPressedEvent event)
	{
		if (event.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			m_Camera->SetRotate(true);
		else if (event.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
			m_Camera->SetMove(true);

		return true;
	}

	bool Renderer::OnMouseButtonRelease(MouseButtonReleasedEvent event)
	{
		if (event.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			m_Camera->SetRotate(false);
		else if (event.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
			m_Camera->SetMove(false);

		return true;
	}

	bool Renderer::OnWindowResize(WindowResizeEvent event)
	{
		auto width = event.GetWidth();
		auto height = event.GetHeight();
		auto x = width * 0.01f * 0.5f;
		auto y = height * 0.01f * 0.5f;
		m_Camera->SetFrustum(-x, x, -y, y);
		m_Camera->SetViewport(0, 0, width, height);
		return true;
	}
}