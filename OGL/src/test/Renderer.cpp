#include "Renderer.h"

#include "Core/Base.h"

#include "Renderer/Shader.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

#include "GLFuncs.h"

namespace Logl
{
#define BIND_FUNC(name) std::bind(&Renderer::name, this, std::placeholders::_1)


	Renderer::Renderer(Window* window, bool bOrtho)
		: m_Shader(nullptr), m_Vao(nullptr)
	{
		m_window = window;
		m_window->SetEventCallback(BIND_FUNC(OnEvent));

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
			auto position = vec3(0.0f, 0.0f, 3.0f);
			m_Camera = new PerspectiveCamera(frustum, position);
		}

		m_tmpParam.lastX = width / 2.0f;
		m_tmpParam.lastY = height / 2.0f;

		m_Running = false;
	}

	Renderer::~Renderer()
	{
		delete m_Camera;
	}

	void Renderer::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::SetShader(Shader& shader)
	{
		m_Shader = &shader;
		m_Shader->Use();
	}

	void Renderer::SetVexterArray(VertexArray& vao)
	{
		m_Vao = &vao;
	}

	void Renderer::Render()
	{
		if (m_Shader == nullptr || m_Vao == nullptr)
		{
			PRINT("no shader to use!");
			return;
		}

		DrawCallFunc drawcall;
		if (m_Vao->IsUsingIndex())
		{
			drawcall = [](int count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); };
		}
		else
		{
			drawcall = [](int count) { glDrawArrays(GL_TRIANGLES, 0, count); };
		}

		vec3 cubePositions[10] = {
		  vec3(0.0f,  0.0f,  0.0f),
		  vec3(2.0f,  5.0f, -15.0f),
		  vec3(-1.5f, -2.2f, -2.5f),
		  vec3(-3.8f, -2.0f, -12.3f),
		  vec3(2.4f, -0.4f, -3.5f),
		  vec3(-1.7f,  3.0f, -7.5f),
		  vec3(1.3f, -2.0f, -2.5f),
		  vec3(1.5f,  2.0f, -2.5f),
		  vec3(1.5f,  0.2f, -1.5f),
		  vec3(-1.3f,  1.0f, -1.5f)
		};

		// Loop
		m_Running = true;
		while (m_Running)
		{
			// per-frame time logic
			m_tmpParam.UpdateTime((float)glfwGetTime());

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Shader->Use();

			//  projection matrix
			auto projection = m_Camera->GetProjectionMatrix();
			m_Shader->SetUniform("projection", projection.ValuePtr());

			// camera/view transformation
			auto view = m_Camera->GetViewMatrix();
			m_Shader->SetUniform("view", view.ValuePtr());

			// render boxes
			m_Vao->Bind();
			for (int i = 0; i < 10; i++)
			{
				//  model matrix
				auto model = mat4::Translate(cubePositions[i]);
				float angle = 20.0f * i;
				model = model * mat4::Rotate(Radians(angle), vec3(1.0f, 0.3f, 0.5f));
				m_Shader->SetUniform("model", model.ValuePtr());

				drawcall(m_Vao->GetCount());
			}

			m_window->OnUpdate();
		}
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
		auto xold = m_tmpParam.lastX;
		auto yold = m_window->GetHeight() - m_tmpParam.lastY; // window coordinates -> viewport coordinates
		auto pair = m_tmpParam.UpdatePos(event.GetXPos(), event.GetYPos());
		m_Camera->Turn(pair.first, pair.second);
		m_Camera->Move(pair.first, pair.second);

		auto xnew = m_tmpParam.lastX;;
		auto ynew = m_window->GetHeight() - m_tmpParam.lastY; // window coordinates -> viewport coordinates
		m_Camera->Move(xold, yold, xnew, ynew);

		return true;
	}

	bool Renderer::OnMouseScroll(MouseScrolledEvent event)
	{
		m_Camera->Scale(event.GetYOffset());

		auto xpos = m_tmpParam.lastX;
		auto ypos = (float)m_window->GetHeight() - m_tmpParam.lastY; // window coordinates -> viewport coordinates
		m_Camera->Scale(event.GetYOffset(), xpos, ypos);

		m_Camera->Scale(event.GetYOffset(), m_tmpParam.lastX, m_tmpParam.lastY, (float)m_window->GetWidth(), (float)m_window->GetHeight());

		return true;
	}

	bool Renderer::OnKeyPress(KeyPressedEvent event)
	{
		auto key = event.GetKey();
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
			m_Camera->Move(MoveDirection::FORWARD, m_tmpParam.deltaTime);
			break;
		}
		case GLFW_KEY_S:
		{
			m_Camera->Move(MoveDirection::BACKWARD, m_tmpParam.deltaTime);
			break;
		}
		case GLFW_KEY_A:
		{
			m_Camera->Move(MoveDirection::LEFT, m_tmpParam.deltaTime);
			break;
		}
		case GLFW_KEY_D:
		{
			m_Camera->Move(MoveDirection::RIGHT, m_tmpParam.deltaTime);
			break;
		}
		default:
			break;
		}

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