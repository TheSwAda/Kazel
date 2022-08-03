#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "LayerStack.h"
#include "Window.h"

namespace Kazel {
class Application {
 public:
  Application(const Kazel::WindowProps& props);
  virtual ~Application();

  void Run();
  void onEvent(Event& e);  //事件处理
  void Close() { m_Running = false; }
  void PushLayer(Layer* layer);
  void PushOverLayer(Layer* layer);
  static Application& Get() { return *m_Instance; }

  Window& GetWindow() { return *m_Window; }

 private:
  bool onWindowClose(WindowCloseEvent& e);
  bool onWindowResize(WindowResizeEvent& e);

  bool onKeyPressed(KeyPressedEvent& e);

  bool m_Minimum = false;
  bool m_Running = true;

  std::unique_ptr<Window> m_Window;
  ImGuiLayer* m_ImGuiLayer;
  LayerStack m_LayerStack;

  float m_LastTime;

  bool m_Cursor = false;

 private:
  static Application* m_Instance;
};

// implement by user
Application* CreateApplication();
}  // namespace Kazel
