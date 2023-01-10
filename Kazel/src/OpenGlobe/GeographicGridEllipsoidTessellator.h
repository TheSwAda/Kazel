#pragma once

#include <Kazel.h>
#include "Ellipsoid.h"

namespace Kazel {
enum class GeographicGridEllipsoidVertexAttributes {
  Position = 1,
  Normal = 2,
  TextureCoordinate = 4,
  All = Position | Normal | TextureCoordinate
};
using GGEVA = GeographicGridEllipsoidVertexAttributes;

inline GGEVA operator&(GGEVA v1, GGEVA v2) {
  return static_cast<GGEVA>(static_cast<int>(v1) & static_cast<int>(v2));
}

/**
 * @brief 地理网格细分算法
 * 
 */
class GeographicGridEllipsoidTessellator {
 public:
  /**
   * @brief 算法实现
   * @param ellipsoid 椭球体
   * @param numberOfSlicePartitions 垂直切线个数
   * @param numberOfStackPartitions 水平切分个数
   * @param vertexAttributes 数据输出类型
   * @return 
  */
  static Mesh Compute(Ellipsoid ellipsoid, int numberOfSlicePartitions, int numberOfStackPartitions,
                      GGEVA vertexAttributes);

 private:
  static void Subdivide(std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices,
                        const glm::uvec3& triangle, int level);

  static int NumberOfTriangles(int numberOfSlicePartitions, int numberOfStackPartitions);

  static int NumberOfVertices(int numberOfSlicePartitions, int numberOfStackPartitions);

  static glm::vec2 ComputeTextureCoordinate(glm::vec3 position);
};
}  // namespace Kazel