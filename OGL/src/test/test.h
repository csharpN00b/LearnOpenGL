#pragma once

#include <glfw/glfw3.h>

#define DECLARE_RENDERER_FUNC(ns) namespace ns { void RenderScene(); }
#define CALL_RENDERER_FUNC(ns) ns::RenderScene()

DECLARE_RENDERER_FUNC(E14)
DECLARE_RENDERER_FUNC(E13)
DECLARE_RENDERER_FUNC(E12)
DECLARE_RENDERER_FUNC(E11)
DECLARE_RENDERER_FUNC(E10)
DECLARE_RENDERER_FUNC(E9)
DECLARE_RENDERER_FUNC(E8)
DECLARE_RENDERER_FUNC(E7)
DECLARE_RENDERER_FUNC(E6)
DECLARE_RENDERER_FUNC(E5)
DECLARE_RENDERER_FUNC(E4)


#define DECLARE_RENDER_FUNC(ns) namespace ns { void RenderScene(GLFWwindow*); }
#define CALL_RENDER_FUNC(ns) ns::RenderScene(window)

DECLARE_RENDER_FUNC(E3)
DECLARE_RENDER_FUNC(E2)
DECLARE_RENDER_FUNC(E1)


void TestLogl();
void TestGLM();
