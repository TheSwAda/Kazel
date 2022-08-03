#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Kazel {
class Camera {
 public:
  virtual ~Camera() {}

  virtual const glm::vec3& GetPosition() const = 0;
  virtual const glm::mat4& GetProjectionMartix() const = 0;
  virtual const glm::mat4& GetViewMatrix() const = 0;
  virtual glm::mat4 GetViewProjectionMatrix() const = 0;
};
}  // namespace Kazel