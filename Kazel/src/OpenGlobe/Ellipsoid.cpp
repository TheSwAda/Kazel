#include "kzpch.h"

#include "Ellipsoid.h"
#include <glm/gtx/transform.hpp>

namespace Kazel {
Ellipsoid::Ellipsoid(const dvec3& radii) {
  if ((radii.x <= 0.0) || (radii.y <= 0.0) || (radii.z <= 0.0)) {
    // #TODO
    // error deal
  }

  m_radii = radii;
  m_radiiSquared = radii * radii;
  m_radiiToTheFourth = radii * radii * radii * radii;
  m_oneOverRadiiSquared = 1.0 / (radii * radii);
}

Kazel::dvec3 Ellipsoid::ToVector3d(const Geodetic3D& geodetic) const {
  auto n = GeodeticSurfaceNormal(geodetic);
  auto k = n * m_radii * m_radii;
  double gamma = glm::length(k * n);

  auto surface = k / gamma;
  return surface + (geodetic.m_height * n);
}

Geodetic3D Ellipsoid::ToGeodetic3D(const dvec3& position) const {
  auto p = ScaleToGeodeticSurface(position);
  auto h = position - p;
  double height = std::copysign(glm::length(h), glm::dot(h, p));
  return Geodetic3D{p.x, p.y, height};
}

dvec3 Ellipsoid::ScaleToGeodeticSurface(dvec3 position) const {
  double beta = 1.0 / glm::length(position / m_radii);
  auto n = beta * position * m_oneOverRadiiSquared;
  double alpha = (1.0 - beta) * (glm::length(position) / glm::length(n));

  auto p2 = position * position;

  double s = 0.0;
  double dSdA = 1.0;

  dvec3 d{0};

  auto sum = [](const dvec3& vec) {
    return vec.x + vec.y + vec.z;
  };

  do {
    alpha -= (s / dSdA);

    auto d = 1.0 + alpha * m_oneOverRadiiSquared;
    s = sum(p2 / (m_radiiSquared * d * d)) - 1.0;

    dSdA = -2.0 * sum(p2 / (m_radiiToTheFourth * d * d * d));
  } while (std::abs(s) > 1e-10);

  return position / d;
}

std::vector<dvec3> Ellipsoid::ComputeCurve(const dvec3& start, const dvec3& stop, double granularity) const {

  auto normal = glm::normalize(glm::cross(start, stop));
  double theta = std::acos(glm::dot(start, stop) / glm::length(start) / glm::length(stop));

  int n = (std::max)(static_cast<int>(theta / granularity - 1), 0);

  std::vector<dvec3> positions(n);
  positions.push_back(start);

  for (int i = 1; i <= n; ++i) {
    double phi = (i * granularity);
    glm::dmat3 transform = glm::rotate(phi, normal);
    auto next = transform * positions[i - 1];
    positions.push_back(ScaleToGeocentricSurface(next));
  }

  return positions;
}

}  // namespace Kazel