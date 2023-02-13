#include "kzpch.h"
#include "Core/Input.h"
#include "PerspectCamera.h"
#include "Core/Log.h"

namespace Kazel {

PerspectiveCamera::PerspectiveCamera() : PerspectiveCamera(30, 1, 0.01, 64) {}

PerspectiveCamera::PerspectiveCamera(double fovy, double aspect, double zNear, double zFar)
    : m_fovy(glm::radians(fovy)), m_apsect(aspect), m_zNear(zNear), m_zFar(zFar) {
  m_fovx = 2.0 * std::atan(aspect * glm::tan(m_fovy * 0.5));
}

void PerspectiveCamera::ZoomToTarget(double radius) {
  auto toEye = glm::normalize(m_position - m_target);

  double sin = std::sin(std::min(m_fovx, m_fovy) * 0.5);
  double distance = (radius / sin);
  m_position = m_target + (distance * toEye);
}

void PerspectiveCamera::UpdateView() {
  glm::dvec3 right = glm::cross(m_target - m_position, m_worldUp);  //摄像机x轴
  m_up = glm::normalize(glm::cross(right, m_target - m_position));  //摄像机y轴
  m_view = glm::lookAt(m_position, m_target, m_up);
}


void PerspectiveCamera::UpdateProjection() {
  m_projection = glm::perspective(m_fovy, m_apsect, m_zNear, m_zFar);
}

//////////////////////////////////////////////////////////////////////////
PerspectCameraController::PerspectCameraController(uint32_t width, uint32_t height, float zNear /*= 0.1f*/,
                                                   float zFar /*= 100.0f*/)
    : m_Camera(45.0f, width * 1.0f / height, zNear, zFar), m_LastX(width / 2.0f), m_LastY(height / 2.0f) {
  m_Camera.UpdateView();
  m_Camera.UpdateProjection();
}

void PerspectCameraController::onUpdate(double timeStep) {
    auto front = m_Camera.Front();
  //比起在事件中回调处理camera的控制,直接在循环中处理,camera的移动更加顺滑
  if (Input::IsKeyPressed(KeyCode::W))
    m_Camera.m_position += m_MoveSpeed * timeStep * front;
  else if (Input::IsKeyPressed(KeyCode::S))
    m_Camera.m_position -= m_MoveSpeed * timeStep * front;

  if (Input::IsKeyPressed(KeyCode::A))
    m_Camera.m_position -= m_MoveSpeed * timeStep * glm::normalize(glm::cross(front, m_Camera.m_up));
  else if (Input::IsKeyPressed(KeyCode::D))
    m_Camera.m_position += m_MoveSpeed * timeStep * glm::normalize(glm::cross(front, m_Camera.m_up));

  m_Camera.UpdateView();
}

void PerspectCameraController::onEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrolledEvent>(KZ_BIND_EVENT_FN(PerspectCameraController::OnMouseScrolled));

  dispatcher.Dispatch<WindowResizeEvent>(KZ_BIND_EVENT_FN(PerspectCameraController::OnWindowResized));

  dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool {
    if (e.GetKeyCode() == KeyCode::Left_control) {
      KZ_CORE_INFO("Camera MouseMove");
      m_MouseShouldMove = !m_MouseShouldMove;
    }
    return false;
  });

  if (m_MouseShouldMove) {
    dispatcher.Dispatch<MouseMovedEvent>(KZ_BIND_EVENT_FN(PerspectCameraController::onMouseMoved));
  }
}

bool PerspectCameraController::OnWindowResized(WindowResizeEvent& event) {
  m_Camera.m_apsect = event.GetWidth() * 1.0f / event.GetHeight();
  m_Camera.UpdateProjection();
  return false;
}

bool PerspectCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
  m_Camera.m_fovy -= event.GetYOffset() * 0.5f;

  if (m_Camera.m_fovy < 0.1f)
    m_Camera.m_fovy = 0.1f;
  if (m_Camera.m_fovy > 90.0f)
    m_Camera.m_fovy = 90.0f;

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

  //m_Camera.m_Yaw += (event.GetX() - m_LastX) * m_Sensitivity;
  //m_Camera.m_Picth += (m_LastY - event.GetY()) * m_Sensitivity;  //向上移动时,y轴是减少的

  //m_Camera.m_Picth = std::min(89.0f, m_Camera.m_Picth);
  //m_Camera.m_Picth = std::max(-89.0f, m_Camera.m_Picth);

  m_LastX = event.GetX();
  m_LastY = event.GetY();

  m_Camera.UpdateView();
  return false;
}
}  // namespace Kazel