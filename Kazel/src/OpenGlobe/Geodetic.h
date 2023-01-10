#pragma once
#include <glm/glm.hpp>

namespace Kazel {
class Geodetic2D {
 public:
  double m_longitude{0};  // 经度
  double m_latitude{0};   // 纬度

  operator glm::dvec2() { return glm::dvec2{m_longitude, m_latitude}; }
};

class Geodetic3D {
 public:
  double m_longitude{0};  // 经度
  double m_latitude{0};   // 纬度
  double m_height{0};     // 高度

  operator glm::dvec3() { return glm::dvec3{m_longitude, m_latitude, m_height}; }
};
}  // namespace Kazel