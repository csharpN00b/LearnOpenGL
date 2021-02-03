
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math/Matrix4f.h"

#include "Renderer/Shader.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

#include "Window/Window.h"
#include "Window/MouseEvent.h"
#include "Window/KeyEvent.h"
#include "Window/WindowEvent.h"

#include "../GLFuncs.h"


//using namespace Logl;

namespace Logl
{
#define BIND_FUNC(name) std::bind(&Renderer::name, this, std::placeholders::_1)

	class Renderer
	{
	public:
		Renderer(Window* window, bool bOrtho)
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

		~Renderer()
		{
			delete m_Camera;
		}

		void Begin()
		{
			glEnable(GL_DEPTH_TEST);

			// Shaders
			Shader shaderProgram("asserts/shaders/mvp_vs.glsl", "asserts/shaders/mvp_fs.glsl");
			if (!shaderProgram.IsValid())
			{
				__debugbreak();
				return;
			}

			// Vertex
			unsigned int vao{}, vbo{}, ebo{};
			auto initGeometryData = [&vao, &vbo, &ebo]()
			{
				float vertices[] = {
				-0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
				 0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				-0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
				};

				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);

				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				int count = sizeof(vertices) / sizeof(vertices[0]) / 8;
				return count;
			};
			int count = initGeometryData();

			// Texture
			unsigned int texture1 = LoadTexture("asserts/textures/container.jpg", GL_RGB);
			unsigned int texture2 = LoadTexture("asserts/textures/awesomeface.png", GL_RGBA);
			if (texture1 == 0 || texture2 == 0)
			{
				PRINT("Faild to load texture!\n");
				__debugbreak();
				return;
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			shaderProgram.Use();
			shaderProgram.SetUniform("texture1", 0);
			shaderProgram.SetUniform("texture2", 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

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

				shaderProgram.Use();

				//  projection matrix
				auto projection = m_Camera->GetProjectionMatrix();
				shaderProgram.SetUniform("projection", projection.ValuePtr());

				// camera/view transformation
				auto view = m_Camera->GetViewMatrix();
				shaderProgram.SetUniform("view", view.ValuePtr());

				// render boxes
				glBindVertexArray(vao);
				for (int i = 0; i < 10; i++)
				{
					//  model matrix
					auto model = mat4::Translate(cubePositions[i]);
					float angle = 20.0f * i;
					model = model * mat4::Rotate(Radians(angle), vec3(1.0f, 0.3f, 0.5f));
					shaderProgram.SetUniform("model", model.ValuePtr());

					glDrawArrays(GL_TRIANGLES, 0, count);
				}

				m_window->OnUpdate();
			}

			glDeleteVertexArrays(1, &vao);
		}

	public:
		void OnEvent(Event& event)
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

		bool OnMouseMove(MouseMovedEvent event)
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

		bool OnMouseScroll(MouseScrolledEvent event)
		{
			m_Camera->Scale(event.GetYOffset());

			auto xpos = m_tmpParam.lastX;
			auto ypos = (float)m_window->GetHeight() - m_tmpParam.lastY; // window coordinates -> viewport coordinates
			m_Camera->Scale(event.GetYOffset(), xpos, ypos);

			m_Camera->Scale(event.GetYOffset(), m_tmpParam.lastX, m_tmpParam.lastY, (float)m_window->GetWidth(), (float)m_window->GetHeight());

			return true;
		}

		bool OnKeyPress(KeyPressedEvent event)
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

		bool OnKeyRelease(KeyReleasedEvent event)
		{
			return true;
		}

		bool OnMouseButtonPress(MouseButtonPressedEvent event)
		{
			if (event.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
				m_Camera->SetRotate(true);
			else if (event.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
				m_Camera->SetMove(true);

			return true;
		}

		bool OnMouseButtonRelease(MouseButtonReleasedEvent event)
		{
			if (event.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
				m_Camera->SetRotate(false);
			else if (event.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
				m_Camera->SetMove(false);

			return true;
		}

		bool OnWindowResize(WindowResizeEvent event)
		{
			auto width = event.GetWidth();
			auto height = event.GetHeight();
			auto x = width * 0.01f * 0.5f;
			auto y = height * 0.01f * 0.5f;
			m_Camera->SetFrustum(-x, x, -y, y);
			m_Camera->SetViewport(0, 0, width, height);
			return true;
		}

	private:
		Window* m_window;
		Camera* m_Camera;
		bool m_Running;

	private:
		struct TmpParam
		{
			float deltaTime; // time between current frame and last frame
			float lastFrame;

			bool firstMouse;
			float lastX;
			float lastY;

			TmpParam()
				: deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0.0f), lastY(0.0f)
			{}

			void UpdateTime(float currentFrame)
			{
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
			}

			std::pair<float, float> UpdatePos(float xpos, float ypos)
			{
				if (firstMouse)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouse = false;
					return { 0.0f, 0.0f };
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
				lastX = xpos;
				lastY = ypos;

				return { xoffset, yoffset };
			}
		};
		
		TmpParam m_tmpParam;
	};
}

namespace E4
{
	void RenderScene(GLFWwindow*)
	{
		Logl::Window window(800, 600, "Hello OpenGL");
		Logl::Renderer renderer(&window, true);
		renderer.Begin();
	}
}
