#pragma once
#include "Camera.h"
#include "Core/Input.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <optional>
namespace Kazel {
//透�?�投影Camera
class PerspectiveCamera : public Camera {
  friend class PerspectCameraController;

 public:
  PerspectiveCamera(float fov, float aspect, float zNear, float zFar);

  const glm::mat4& GetProjectionMartix() const override {
    return m_ProjectionMartix;
  }

  const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }

  glm::mat4 GetViewProjectionMatrix() const override {
    return m_ProjectionMartix * m_ViewMatrix;
  }

  const glm::vec3& GetPosition() const override { return m_Position; }
  const glm::vec3& GetFront() const { return m_Front; }
  const glm::vec3& GetUp() const { return m_Up; }

 protected:
  void UpdateView();
  void UpdateProjection();

 private:
  float m_Fov;
  float m_Apsect;
  float m_ZNear;
  float m_ZFar;

  glm::mat4 m_ProjectionMartix;  //投影矩阵
  glm::mat4 m_ViewMatrix;        //观察矩阵

  glm::vec3 m_Position;
  glm::vec3 m_Front;  //摄像机指向的反方�?(z轴�?�方�?,单位向量)
  glm::vec3 m_Up;

  glm::vec3 m_WorldUp;

  float m_Picth = 0;     //�?�?(统一逆时针移动时变大)
  float m_Yaw = -90.0f;  //偏航
};

class PerspectCameraController {
 public:
  PerspectCameraController(uint32_t width, uint32_t height, float zNear = 0.1f,
                           float zFar = 100.0f);

  void onUpdate(float timeStep);

  PerspectiveCamera& GetCamera() { return m_Camera; }
  const PerspectiveCamera& GetCamera() const { return m_Camera; }

  void setPosition(glm::vec3 pos);
  void onEvent(Event& event);

  bool OnWindowResized(WindowResizeEvent& event);
  bool OnMouseScrolled(MouseScrolledEvent& event);
  bool onKeyPressed(KeyPressedEvent& event);
  bool onMouseMoved(MouseMovedEvent& event);

 private:
  bool m_MouseShouldMove = false;

  PerspectiveCamera m_Camera;

  float m_LastX;
  float m_LastY;

  float m_MoveSpeed = 10.0f;
  float m_Sensitivity = 0.05f;
  float m_TimeStep = 0;
};
}  // namespace Kazel
