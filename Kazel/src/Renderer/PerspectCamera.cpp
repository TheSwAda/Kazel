#include "kzpch.h"

#include "PerspectCamera.h"
#include "Core/Log.h"

namespace Kazel {
PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float zNear,
                                     float zFar)
    : m_Fov(fov),
      m_Apsect(aspect),
      m_ZNear(zNear),
      m_ZFar(zFar),
      m_Position(0, 0, 3.0f),
      m_WorldUp(0, 1.0f, 0) {
  UpdateView();
  UpdateProjection();
}

void PerspectiveCamera::UpdateView() {
  //计算俯仰角和偏航角获取摄像机坐标轴
  glm::vec3 front;
  front.x = cos(glm::radians(m_Picth)) * cos(glm::radians(m_Yaw));
  front.y = sin(glm::radians(m_Picth));
  front.z = cos(glm::radians(m_Picth)) * sin(glm::radians(m_Yaw));
  m_Front = glm::normalize(front);

  glm::vec3 right = glm::normalize(glm::cross(m_Front, m_WorldUp));  //摄像机x轴
  m_Up = glm::normalize(glm::cross(right, m_Front));  //摄像机y轴

  m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void PerspectiveCamera::UpdateProjection() {
  m_ProjectionMartix =
      glm::perspective(glm::radians(m_Fov), m_Apsect, m_ZNear, m_ZFar);
}

PerspectCameraController::PerspectCameraController(uint32_t width,
                                                   uint32_t height,
                                                   float zNear /*= 0.1f*/,
                                                   float zFar /*= 100.0f*/)
    : m_Camera(45.0f, width * 1.0f / height, zNear, zFar),
      m_LastX(width / 2.0f),
      m_LastY(height / 2.0f) {}

void PerspectCameraController::onUpdate(float timeStep) {
  //比起在事件中回调处理camera的控制,直接在循环中处理,camera的移动更加顺滑
  if (Input::IsKeyPressed(Key::W))
    m_Camera.m_Position += m_MoveSpeed * timeStep * m_Camera.m_Front;
  else if (Input::IsKeyPressed(Key::S))
    m_Camera.m_Position -= m_MoveSpeed * timeStep * m_Camera.m_Front;

  if (Input::IsKeyPressed(Key::A))
    m_Camera.m_Position -=
        m_MoveSpeed * timeStep *
        glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up));
  else if (Input::IsKeyPressed(Key::D))
    m_Camera.m_Position +=
        m_MoveSpeed * timeStep *
        glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up));

  m_Camera.UpdateView();
}

void PerspectCameraController::setPosition(glm::vec3 pos) {
  m_Camera.m_Position = pos;
  m_Camera.UpdateView();
}

void PerspectCameraController::onEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrolledEvent>(
      KZ_BIND_EVENT_FN(PerspectCameraController::OnMouseScrolled));

  dispatcher.Dispatch<WindowResizeEvent>(
      KZ_BIND_EVENT_FN(PerspectCameraController::OnWindowResized));

  dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool {
    if (e.GetKeyCode() == Key::Left_control) {
      KZ_CORE_INFO("Camera MouseMove");
      m_MouseShouldMove = !m_MouseShouldMove;
    }
    return false;
  });

  if (m_MouseShouldMove) {
    dispatcher.Dispatch<MouseMovedEvent>(
        KZ_BIND_EVENT_FN(PerspectCameraController::onMouseMoved));
  }
}

bool PerspectCameraController::OnWindowResized(WindowResizeEvent& event) {
  m_Camera.m_Apsect = event.GetWidth() * 1.0f / event.GetHeight();
  m_Camera.UpdateProjection();
  return false;
}

bool PerspectCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
  m_Camera.m_Fov -= event.GetYOffset() * 0.5f;

  if (m_Camera.m_Fov < 0.1f) m_Camera.m_Fov = 0.1f;
  if (m_Camera.m_Fov > 90.0f) m_Camera.m_Fov = 90.0f;

  m_Camera.UpdateProjection();
  return false;
}

bool PerspectCameraController::onKeyPressed(KeyPressedEvent& event) {
  return false;
}

bool PerspectCameraController::onMouseMoved(MouseMovedEvent& event) {
  static bool bFirst = true;
  if (bFirst) {
    m_LastX = event.GetX();
    m_LastY = event.GetY();
    bFirst = false;
  }

  m_Camera.m_Yaw += (event.GetX() - m_LastX) * m_Sensitivity;
  m_Camera.m_Picth +=
      (m_LastY - event.GetY()) * m_Sensitivity;  //向上移动时,y轴是减少的

  m_Camera.m_Picth = std::min(89.0f, m_Camera.m_Picth);
  m_Camera.m_Picth = std::max(-89.0f, m_Camera.m_Picth);

  m_LastX = event.GetX();
  m_LastY = event.GetY();

  m_Camera.UpdateView();
  return false;
}
}  // namespace Kazel