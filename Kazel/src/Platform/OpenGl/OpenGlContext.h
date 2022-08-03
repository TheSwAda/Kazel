#pragma once

#include "GLFW/glfw3.h"
#include "Renderer/GraphicsContext.h"

namespace Kazel {
class OpenGlContext : public GraphicsContext {
 public:
  OpenGlContext(GLFWwindow* window);

  void Init() override;

  void SwapBuffer() override;

 private:
  GLFWwindow* m_WindowHandle;
};
}  // namespace Kazel
