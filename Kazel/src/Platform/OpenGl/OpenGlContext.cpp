#include "kzpch.h"

#include "OpenGlContext.h"
#include <glad/glad.h>

#include "Core/Log.h"

namespace Kazel {
OpenGlContext::OpenGlContext(GLFWwindow* window) : m_WindowHandle(window) {
  KZ_CORE_ASSERT(window, "window is null!");
}

void OpenGlContext::Init() {
  glfwMakeContextCurrent(m_WindowHandle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  KZ_CORE_ASSERT(status, "Failed to Initialize GLAD");

  KZ_CORE_INFO("OpenGl Info");
  KZ_CORE_INFO("  Vendor {0}", glGetString(GL_VENDOR));
  KZ_CORE_INFO("  Renderer {0}", glGetString(GL_RENDERER));
  KZ_CORE_INFO("  Version {0}", glGetString(GL_VERSION));
}

void OpenGlContext::SwapBuffer() { glfwSwapBuffers(m_WindowHandle); }
}  // namespace Kazel