#pragma once
#include "Camera.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glm/ext.hpp>

namespace Kazel {
/**
 * @brief 透视投影Camera
*/
class PerspectiveCamera : public Camera {
 public:
  //定义视锥体
  double m_fovy;
  double m_apsect;
  double m_zNear;
  double m_zFar;
  double m_fovx;

  //定义相机的本体坐标系
  glm::dvec3 m_position{3, 0, 0};  //相机的位置
  glm::dvec3 m_target{0, 0, 0};    //相机看向的目标点
  glm::dvec3 m_up{0, 1, 0};        //相机向上指向

  glm::dvec3 m_worldUp{0, 1, 0};

 public:
  PerspectiveCamera();
  PerspectiveCamera(double fovy, double aspect, double zNear, double zFar);

  glm::dvec3 Front() const { return m_target - m_position; }
  double Distance() const { return glm::length(m_target - m_position); }
  void ZoomToTarget(double radius);
  void UpdateView();
  inline void UpdateProjection();
};

class PerspectCameraController {
 public:
  PerspectCameraController(uint32_t width, uint32_t height, float zNear = 0.1f, float zFar = 100.0f);

  void onUpdate(double timeStep);

  PerspectiveCamera& GetCamera() { return m_Camera; }
  const PerspectiveCamera& GetCamera() const { return m_Camera; }

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
