#include "kzpch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

#include "Events/ApplicationEvent.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include "glm/glm.hpp"

namespace Kazel {
Application *Application::m_Instance = nullptr;

Application::Application(const Kazel::WindowProps &props) {
  KZ_CORE_ASSERT(!m_Instance, "application already exists");
  m_Instance = this;
  Kazel::Log::Init();
  m_Window = std::unique_ptr<Window>(Window::Create(props));
  m_Window->SetEventCallback(KZ_BIND_EVENT_FN(Application::onEvent));
  m_Window->SetVSync(true);

  Renderer::Init();  //渲染器初始化
  m_ImGuiLayer = new ImGuiLayer;
  PushOverLayer(m_ImGuiLayer);
}

Application::~Application() { Renderer::ShutDown(); }

void Application::Run() {
  while (m_Running) {
    float time = (float)glfwGetTime();
    float timeStep = time - m_LastTime;
    m_LastTime = time;

    m_Window->Begin();
    if (!m_Minimum) {
      for (Layer *layer : m_LayerStack) layer->OnUpdate(timeStep);

      m_ImGuiLayer->Begin();
      for (Layer *layer : m_LayerStack) layer->OnImGuiRender();
      m_ImGuiLayer->End();
    }
    m_Window->End();
  }

  for (Layer *layer : m_LayerStack) layer->OnDetach();
}

void Application::onEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(
      KZ_BIND_EVENT_FN(Application::onWindowClose));

  dispatcher.Dispatch<WindowResizeEvent>(
      KZ_BIND_EVENT_FN(Application::onWindowResize));

  dispatcher.Dispatch<KeyPressedEvent>(
      KZ_BIND_EVENT_FN(Application::onKeyPressed));

  //将事件传递给其他层
  for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
    if (e.Handled) break;
    (*--it)->OnEvent(e);
  }
}

void Application::PushLayer(Layer *layer) {
  m_LayerStack.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverLayer(Layer *layer) {
  m_LayerStack.PushOverLayer(layer);
  layer->OnAttach();
}

bool Application::onWindowClose(WindowCloseEvent &e) {
  m_Running = false;
  return true;
}

bool Application::onWindowResize(WindowResizeEvent &e) {
  if (e.GetHeight() == 0 || e.GetWidth() == 0) {
    //停止渲染
    m_Minimum = true;
    return true;
  }
  m_Minimum = false;
  Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
  return false;
}

bool Application::onKeyPressed(KeyPressedEvent &e) {
  if (e.GetKeyCode() == Key::Escape)
    m_Running = false;
  else if (e.GetKeyCode() == Key::Left_control) {
    m_Cursor = !m_Cursor;
    m_Window->SetInputMode(
        GLFW_CURSOR, m_Cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  }
  return false;
}
}  // namespace Kazel