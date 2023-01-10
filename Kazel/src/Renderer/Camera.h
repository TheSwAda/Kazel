#pragma once

#include <glm/glm.hpp>

namespace Kazel {

class Camera {
 public:
  Camera() = default;
  Camera(const glm::mat4& projection) : m_projection(projection) {}

  virtual ~Camera() = default;

  const glm::mat4& Projection() const { return m_projection; }
  const glm::mat4& View() const { return m_view; }
  const glm::mat4& ViewPorjection() const { return m_projection * m_view; }

 protected:
  glm::mat4 m_projection{1.0f};
  glm::mat4 m_view{1.0f};
};

}  // namespace Kazel