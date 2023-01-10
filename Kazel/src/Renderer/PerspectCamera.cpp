#include "kzpch.h"

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
  glm::dvec3 right = glm::cross(m_target - m_position, m_worldUp);  //…„œÒª˙x÷·
  m_up = glm::normalize(glm::cross(right, m_target - m_position));  //…„œÒª˙y÷·
  m_view = glm::lookAt(m_position, m_target, m_up);
}

}  // namespace Kazel