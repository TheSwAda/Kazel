#pragma once
#include "Camera.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include <glm/ext.hpp>

namespace Kazel {
//正交投影Camera
class OrthographicCamera : public Camera {
  friend class OrthographicCameraController;

 public:
  OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f,
                     float zFar = 1.0f)
      : Camera(glm::ortho(left, right, bottom, top, zNear, zFar)), m_position(0) {
    UpdateCamera();
  }

  void setProjection(float left, float right, float bottom, float top) {
    m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  }

  void SetPosition(const glm::vec3 position) {
    m_position = position;
    UpdateCamera();
  }
  const glm::vec3& GetPosition() const { return m_position; }

  void SetRotation(float rotation) {
    m_Rotation = rotation;
    UpdateCamera();
  }
  float GetRotation() { return m_Rotation; }

 protected:
  void UpdateCamera();

 private:
  glm::vec3 m_position;
  float m_Rotation = 0;
};

class OrthographicCameraController {
 public:
  OrthographicCameraController(float aspectRadio);

  void onUpdate(float timeStep);
  OrthographicCamera& GetCamera() { return m_Camera; }
  const OrthographicCamera& GetCamera() const { return m_Camera; }

  void onEvent(Event& event);
  bool OnMouseScrolled(MouseScrolledEvent& event);
  bool onWindowResized(WindowResizeEvent& event);

 private:
  float m_AspectRatio;
  float m_ZoomLevel = 1.0f;

  glm::vec3 m_CameraPosition;
  float m_Rotation = .0f;
  float m_MoveSpeed = 1.0f;

  OrthographicCamera m_Camera;
};
}  // namespace Kazel
