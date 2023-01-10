#pragma once
#include <vector>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>

#include "Geodetic.h"

namespace Kazel {
using glm::dvec3;
/**
 * @brief 椭园体的表示
 * 这里纬度指的是赤道平面和大地法线的夹角
 */
class Ellipsoid {
  dvec3 m_radii;

 public:
  dvec3 m_radiiSquared;
  dvec3 m_radiiToTheFourth;
  dvec3 m_oneOverRadiiSquared;

  constexpr static dvec3 Wgs84{6378137.0, 6378137.0, 6356752.314245};
  constexpr static dvec3 ScaledWgs84{1.0, 1.0, 6356752.314245 / 6378137.0};
  constexpr static dvec3 UnitSphere{1.0, 1.0, 1.0};

 public:
  Ellipsoid(double x, double y, double z) : Ellipsoid(dvec3(x, y, z)) {}

  Ellipsoid(const dvec3& radii);

  /**
   * @brief 
   * @return 
  */
  const dvec3 Radii() const { return m_radii; }
  const double MaximumRadius() const { return glm::max(m_radii.x, m_radii.y, m_radii.z); }

 public:
  /**
   * @brief 计算基于地心的表面法线方向
   *
   * @param positionOnEllipsoid 在笛卡尔坐标系下的位置坐标
   * @return Vector3d
   */
  dvec3 CentricSurfaceNormal(const dvec3& positionOnEllipsoid) const { return glm::normalize(positionOnEllipsoid); }

  /**
   * @brief 计算基于地球表面法线方向
   *
   * @param positionOnEllipsoid 在笛卡尔坐标系下的位置坐标
   * @return Array3d
   */
  dvec3 GeodeticSurfaceNormal(const dvec3& positionOnEllipsoid) const {
    return glm::normalize(positionOnEllipsoid * m_radiiSquared);
  }

  /**
   * @brief 计算基于地球表面法线方向
   *
   * @param geodetic 大地坐标系下的经纬高
   * @return Array3d
   */
  dvec3 GeodeticSurfaceNormal(const Geodetic3D& geodetic) const {
    double cosLat = std::cos(geodetic.m_longitude);
    return dvec3(cosLat * std::cos(geodetic.m_latitude), cosLat * std::sin(geodetic.m_latitude), cosLat);
  }
  /**
   * @brief Geographic to WGS84;
   *
   * @param geodetic 大地坐标系下的经纬高
   * @return Array3d
   */
  dvec3 ToVector3d(const Geodetic3D& geodetic) const;

  /**
   * @brief 给定笛卡尔坐标系下的某一点，获取它的经纬高
   *
   * @param position基于笛卡尔坐标系下的坐标值
   * @return Geodetic3D
   */
  Geodetic3D ToGeodetic3D(const dvec3& position) const;

  /**
   * @brief 给定椭球体上的某一点，获取它的经纬度
   *
   * @param position 基于笛卡尔坐标系下的坐标值
   * @return Geodetic2D
   */
  Geodetic2D ToGeodetic2D(const dvec3& position) const {
    auto n = GeodeticSurfaceNormal(position);
    return Geodetic2D{std::atan(n.y / n.x), std::asin(n.z / n.length())};
  }

  /**
   * @brief 给定笛卡尔坐标系下的某一点，计算出它于椭球体的交点（基于球心）
   *
   * @param position 基于笛卡尔坐标系下的坐标值
   * @return Array3d
   */
  dvec3 ScaleToGeocentricSurface(const dvec3& position) const {
    auto k = position * m_radii;
    constexpr double beta = 1.0 / k.length();
    return beta * position;
  }

  /**
   * @brief 给定笛卡尔坐标系下的某一点，计算出它于椭球体的交点（沿着大地表面法线）
   *  使用牛顿迭代法计算。
   * @param position 基于笛卡尔坐标系下的坐标值
   * @return Array3d
   */
  dvec3 ScaleToGeodeticSurface(dvec3 position) const;

  /**
   * @brief 计算两点之间的曲线坐标值
   * 
   * @param start 笛卡尔坐标系下的坐标
   * @param stop 
   * @param granularity 细粒度（弧度）
   * @return std::vector<Array3d> 
   */
  std::vector<dvec3> ComputeCurve(const dvec3& start, const dvec3& stop, double granularity) const;
};
}  // namespace Kazel