#pragma once

#include <glm/glm.hpp>

namespace Kazel {

class Camera {
 public:
  Camera() = default;
  Camera(const glm::dmat4& projection) : m_projection(projection) {}

  virtual ~Camera() = default;

  const glm::mat4& Projection() const { return glm::mat4(m_projection); }
  const glm::mat4& View() const { return glm::mat4(m_view); }
  const glm::mat4& ViewPorjection() const { return glm::mat4(m_projection * m_view); }

 protected:
  glm::dmat4 m_projection{1.0f};
  glm::dmat4 m_view{1.0f};
};

}  // namespace Kazel