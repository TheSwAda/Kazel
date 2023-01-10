#pragma once
#include "Camera.h"

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
  glm::dvec3 m_position{0, 0, 3};  //相机的位置
  glm::dvec3 m_target{0, 0, 0};      //相机看向的目标点
  glm::dvec3 m_up{0, 1, 0};          //相机向上指向

  glm::dvec3 m_worldUp{0, 1.0, 0};

 public:
  PerspectiveCamera();
  PerspectiveCamera(double fovy, double aspect, double zNear, double zFar);

  glm::dvec3 Front() const { return m_target - m_position; }
  double Distance() const { return glm::length(m_target - m_position); }
  void ZoomToTarget(double radius);
  void UpdateView();
  void UpdateProjection() { m_projection = glm::perspective(m_fovy, m_apsect, m_zNear, m_zFar); }
};

}  // namespace Kazel
