#include "kzpch.h"

#include "OrthographicCamera.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include <glm/gtx/transform.hpp> 
namespace Kazel {
void OrthographicCamera::UpdateCamera() {
  glm::mat4 transform =
      glm::translate(m_position) * glm::rotate(glm::radians(m_Rotation), glm::vec3(0, 0, 1.0f));

  //在世界坐标系中,camera和场景的位置已经决定,
  //从camera的视角看向物体,相当于以camera的位置为坐标原点,场景物体的坐标也不同于世界坐标系
  //物体是从世界坐标系转到了以camera为中心的观察坐标系
  m_view = glm::inverse(transform);
}

OrthographicCameraController::OrthographicCameraController(float aspectRadio)
    : m_AspectRatio(aspectRadio),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
               m_ZoomLevel),
      m_CameraPosition(0) {}

void OrthographicCameraController::onUpdate(float timeStep) {
  if (Input::IsKeyPressed(KeyCode::W))
    m_CameraPosition.y += m_MoveSpeed * timeStep;
  else if (Input::IsKeyPressed(KeyCode::S))
    m_CameraPosition.y -= m_MoveSpeed * timeStep;

  if (Input::IsKeyPressed(KeyCode::A))
    m_CameraPosition.x -= m_MoveSpeed * timeStep;
  else if (Input::IsKeyPressed(KeyCode::D))
    m_CameraPosition.x += m_MoveSpeed * timeStep;

  m_Camera.SetPosition(m_CameraPosition);
}

void OrthographicCameraController::onEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrolledEvent>(
      KZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(
      KZ_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
  m_ZoomLevel -= event.GetYOffset() * 0.25f;

  m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
  m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                         m_ZoomLevel);
  return false;
}

bool OrthographicCameraController::onWindowResized(WindowResizeEvent& event) {
  m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
  m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                         m_ZoomLevel);
  return false;
}
}  // namespace Kazel