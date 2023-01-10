#include "kzpch.h"

#include "WindowsWindow.h"

#include <glad/glad.h>

#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGl/OpenGlContext.h"

namespace Kazel {
static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description) {
  KZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window* Window::Create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
  Init(props);
}

WindowsWindow::~WindowsWindow() {
  Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
  m_Data.Props = props;

  KZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

  if (s_GLFWWindowCount == 0) {
    int success = glfwInit();
    KZ_CORE_ASSERT(success, "Could not init GLFW!!");
    glfwSetErrorCallback(GLFWErrorCallback);
  }

  m_Window =
      glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
  ++s_GLFWWindowCount;
  m_Context = new OpenGlContext(m_Window);
  m_Context->Init();

  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);
  // Set GLFW callbacks
#pragma region WindowResize
  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.Props.Width = width;
    data.Props.Height = height;

    WindowResizeEvent event(width, height);
    data.EventCallback(event);
  });
#pragma endregion

#pragma region WindowClose
  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
  });
#pragma endregion

#pragma region KeyBoard
  glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(static_cast<KeyCode>(key), 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(static_cast<KeyCode>(key));
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(static_cast<KeyCode>(key), 1);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t key) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    KeyTypedEvent event(static_cast<KeyCode>(key));
    data.EventCallback(event);
  });
#pragma endregion

#pragma region mouse
  glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(static_cast<MouseCode>(button), xpos, ypos);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(static_cast<MouseCode>(button), xpos, ypos);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event(xOffset, yOffset);
    data.EventCallback(event);
  });

  glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    MouseMovedEvent event(xPos, yPos);
    data.EventCallback(event);
  });
#pragma endregion
}

void WindowsWindow::Shutdown() {
  glfwDestroyWindow(m_Window);
  --s_GLFWWindowCount;

  if (s_GLFWWindowCount == 0) {
    glfwTerminate();
  }
}

void WindowsWindow::Begin() {
  ClearState state;
  RenderCommand::Clear(state);
}

void WindowsWindow::End() {
  glfwPollEvents();  //处理事件 onEvent()
  m_Context->SwapBuffer();
}

void WindowsWindow::SetVSync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
  return m_Data.VSync;
}

void WindowsWindow::SetInputMode(int mode, int value) {
  glfwSetInputMode(m_Window, mode, value);
}
}  // namespace Kazel