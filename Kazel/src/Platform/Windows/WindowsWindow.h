#pragma once

#include <GLFW/glfw3.h>
#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"

namespace Kazel {
class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void Begin() override;
  void End() override;

  unsigned int GetWidth() const override { return m_Data.Props.Width; }
  unsigned int GetHeight() const override { return m_Data.Props.Height; }

  // Window attributes
  void SetEventCallback(const EventCallbackFn& callback) override {
    m_Data.EventCallback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;
  void SetInputMode(int mode, int value) override;

  virtual void* GetNativeWindow() const { return m_Window; }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* m_Window;
  GraphicsContext* m_Context;

  struct WindowData {
    WindowProps Props;
    bool VSync;

    EventCallbackFn EventCallback;  //事件处理函数
  };

  WindowData m_Data;
};
}  // namespace Kazel