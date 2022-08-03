#pragma once
#include "Camera.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Kazel {
//正交投影Camera
class OrthographicCamera : public Camera {
  friend class OrthographicCameraController;

 public:
  OrthographicCamera(float left, float right, float bottom, float top,
                     float zNear = -1.0f, float zFar = 1.0f)
      : m_ProjectionMartix(glm::ortho(left, right, bottom, top, zNear, zFar)),
        m_Position(0) {
    UpdateCamera();
  }

  void setProjection(float left, float right, float bottom, float top) {
    m_ProjectionMartix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  }

  void SetPosition(const glm::vec3 position) {
    m_Position = position;
    UpdateCamera();
  }
  const glm::vec3& GetPosition() const override { return m_Position; }

  void SetRotation(float rotation) {
    m_Rotation = rotation;
    UpdateCamera();
  }
  float GetRotation() { return m_Rotation; }

  const glm::mat4& GetProjectionMartix() const override {
    return m_ProjectionMartix;
  }
  const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
  glm::mat4 GetViewProjectionMatrix() const override {
    return m_ProjectionMartix * m_ViewMatrix;
  }

 protected:
  void UpdateCamera();

 private:
  glm::mat4 m_ProjectionMartix;
  glm::mat4 m_ViewMatrix;

  glm::vec3 m_Position;
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
