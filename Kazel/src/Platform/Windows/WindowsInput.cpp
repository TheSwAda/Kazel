#include "kzpch.h"

#include "WindowsInput.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Kazel {
Input* Input::s_Instance = new WindowsInput;

bool WindowsInput::IsKeyPressedImpl(KeyCode keyCode) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());

  auto state = glfwGetKey(window, keyCode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());

  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float WindowsInput::GetMouseXImpl() {
  auto [x, y] = GetMousePosImpl();
  return y;
}

float WindowsInput::GetMouseYImpl() {
  auto [x, y] = GetMousePosImpl();
  return x;
}

std::pair<float, float> WindowsInput::GetMousePosImpl() {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());

  double xPos = 0, yPos = 0;
  glfwGetCursorPos(window, &xPos, &yPos);
  return std::make_pair((float)xPos, (float)yPos);
}
}  // namespace Kazel