#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include "Events/Event.h"

namespace Kazel {
struct WindowProps {
  std::string Title;
  uint32_t Width;
  uint32_t Height;
  glm::vec4 Color;

  WindowProps(const std::string& title = "Kazel Engine", uint32_t width = 1600,
              uint32_t height = 900, glm::vec4 color = glm::vec4{1.0f})
      : Title(title), Width(width), Height(height), Color(color) {}

  WindowProps& operator=(const WindowProps& prop) {
    this->Title = prop.Title;
    this->Width = prop.Width;
    this->Height = prop.Height;
    return *this;
  }
};

// Interface representing a desktop system based Window
class Window {
 public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window() = default;

  virtual void Begin() = 0;
  virtual void End() = 0;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  // Window attributes
  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void SetInputMode(int mode, int value) = 0;
  // in window return GLFW
  virtual void* GetNativeWindow() const = 0;

  static Window* Create(const WindowProps& props = WindowProps());
};
}  // namespace Kazel
