#include "kzpch.h"

#include "GeographicGridEllipsoidTessellator.h"

namespace Kazel {

Mesh GeographicGridEllipsoidTessellator::Compute(Ellipsoid ellipsoid, int numberOfSlicePartitions,
                                                 int numberOfStackPartitions,
                                                 GGEVA vertexAttributes) {

  Ref<VertexArray> vao = VertexArray::Create();

  int numberOfVertices = NumberOfVertices(numberOfSlicePartitions, numberOfStackPartitions);

  //
  // Create lookup table
  //
  std::vector<double> cosTheta(numberOfSlicePartitions);
  std::vector<double> sinTheta(numberOfSlicePartitions);

  for (int j = 0; j < numberOfSlicePartitions; ++j) {
    double theta = glm::two_pi<double>() * (j * 1.0 / numberOfSlicePartitions);
    cosTheta[j] = std::cos(theta);
    sinTheta[j] = std::sin(theta);
  }

  //
  // Create positions
  //
  std::vector<glm::dvec3> positions;
  positions.reserve(numberOfVertices);
  positions.push_back({0, 0, ellipsoid.Radii().z});

  for (int i = 1; i < numberOfStackPartitions; ++i) {
    double phi = glm::pi<double>() * (i * 1.0 / numberOfStackPartitions);
    double sinPhi = std::sin(phi);

    double xSinPhi = ellipsoid.Radii().x * sinPhi;
    double ySinPhi = ellipsoid.Radii().y * sinPhi;
    double zCosPhi = ellipsoid.Radii().z * std::cos(phi);

    for (int j = 0; j < numberOfSlicePartitions; ++j) {
      positions.push_back({cosTheta[j] * xSinPhi, sinTheta[j] * ySinPhi, zCosPhi});
    }
  }
  positions.push_back({0, 0, -ellipsoid.Radii().z});

  Ref<VertexBuffer> vbPositions =
      VertexBuffer::CreateForDouble(glm::value_ptr(positions[0]), 3 * positions.size());
  vbPositions->SetLayout({{Kazel::ShaderDataType::Float3, "positions"}});
  vao->AddVertexBuffer(vbPositions);

  // 创建法线
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> textureCoordinates;
  positions.reserve(numberOfVertices);
  for (int i = 0; i < positions.size(); ++i) {
    auto deticSurfaceNormal = ellipsoid.GeodeticSurfaceNormal(positions[i]);

    normals.emplace_back(glm::vec3(deticSurfaceNormal));

    textureCoordinates.emplace_back(ComputeTextureCoordinate(deticSurfaceNormal));
  }

  if ((vertexAttributes & GGEVA::Normal) == GGEVA::Normal) {
    Ref<VertexBuffer> vbNormals =
        VertexBuffer::Create(glm::value_ptr(normals[0]), 3 * normals.size() * sizeof(float));
    vbNormals->SetLayout({{Kazel::ShaderDataType::Float3, "normals"}});
    vao->AddVertexBuffer(vbNormals);
  }

  if ((vertexAttributes & GGEVA::TextureCoordinate) == GGEVA::TextureCoordinate) {
    Ref<VertexBuffer> vbTextureCoordinates = VertexBuffer::Create(
        glm::value_ptr(textureCoordinates[0]), 2 * textureCoordinates.size() * sizeof(float));
    vbTextureCoordinates->SetLayout({{Kazel::ShaderDataType::Float2, "textureCoordinates"}});
    vao->AddVertexBuffer(vbTextureCoordinates);
  }

  //创建索引
  //
  // Triangle fan top row
  //

  std::vector<glm::u32vec3> indices;
  indices.reserve(3 * NumberOfTriangles(numberOfSlicePartitions, numberOfStackPartitions));
  for (int j = 1; j < numberOfSlicePartitions; ++j) {
    indices.push_back({0, j, j + 1});
  }
  indices.push_back({0, numberOfSlicePartitions, 1});

  //
  // Middle rows are triangle strips
  //
  for (int i = 0; i < numberOfStackPartitions - 2; ++i) {
    int topRowOffset = (i * numberOfSlicePartitions) + 1;
    int bottomRowOffset = ((i + 1) * numberOfSlicePartitions) + 1;

    for (int j = 0; j < numberOfSlicePartitions - 1; ++j) {
      indices.push_back({bottomRowOffset + j, bottomRowOffset + j + 1, topRowOffset + j + 1});
      indices.push_back({bottomRowOffset + j, topRowOffset + j + 1, topRowOffset + j});
    }
    indices.push_back(
        {bottomRowOffset + numberOfSlicePartitions - 1, bottomRowOffset, topRowOffset});
    indices.push_back({bottomRowOffset + numberOfSlicePartitions - 1, topRowOffset,
                       topRowOffset + numberOfSlicePartitions - 1});
  }

  //
  // Triangle fan bottom row
  //
  uint32_t lastPosition = positions.size() - 1;
  for (uint32_t j = lastPosition - 1; j > lastPosition - numberOfSlicePartitions; --j) {
    indices.push_back({lastPosition, j, j - 1});
  }
  indices.push_back({lastPosition, lastPosition - numberOfSlicePartitions, lastPosition - 1});

  Ref<IndexBuffer> ib = IndexBuffer::Create(glm::value_ptr(indices[0]), 3 * indices.size());
  vao->SetIndexBuffer(ib);

  Mesh mesh;
  mesh.m_primitiveType = PrimitiveType::Triangles;
  mesh.m_va = vao;

  return mesh;
}

void GeographicGridEllipsoidTessellator::Subdivide(std::vector<glm::vec3>& positions,
                                                   std::vector<uint32_t>& indices,
                                                   const glm::uvec3& triangle, int level) {
  if (level > 0) {
    auto p1 = (positions[triangle.x] + positions[triangle.y]) * 0.5f;
    positions.emplace_back(glm::normalize(p1));
    auto xx = positions[triangle.y] + positions[triangle.z];
    auto p2 = (positions[triangle.y] + positions[triangle.z]) * 0.5f;
    positions.emplace_back(glm::normalize(p2));
    auto p3 = (positions[triangle.z] + positions[triangle.x]) * 0.5f;
    positions.emplace_back(glm::normalize(p3));

    int i01 = positions.size() - 3;
    int i12 = positions.size() - 2;
    int i20 = positions.size() - 1;

    //
    // Subdivide input triangle into four triangles
    //
    --level;
    Subdivide(positions, indices, {triangle.x, i01, i20}, level);
    Subdivide(positions, indices, {i01, triangle.y, i12}, level);
    Subdivide(positions, indices, {i01, i12, i20}, level);
    Subdivide(positions, indices, {i20, i12, triangle.z}, level);
  } else {
    indices.push_back(triangle.x);
    indices.push_back(triangle.y);
    indices.push_back(triangle.z);
  }
}

int GeographicGridEllipsoidTessellator::NumberOfTriangles(int numberOfSlicePartitions,
                                                          int numberOfStackPartitions) {
  int numberOfTriangles = 2 * numberOfSlicePartitions;  // Top and bottom fans
  numberOfTriangles +=
      2 * ((numberOfStackPartitions - 2) * numberOfSlicePartitions);  // Middle triangle strips
  return numberOfTriangles;
}

int GeographicGridEllipsoidTessellator::NumberOfVertices(int numberOfSlicePartitions,
                                                         int numberOfStackPartitions) {
  return 2 + ((numberOfStackPartitions - 1) * numberOfSlicePartitions);
}

glm::vec2 GeographicGridEllipsoidTessellator::ComputeTextureCoordinate(glm::vec3 position) {
  return glm::vec2((std::atan2(position.y, position.x) / glm::two_pi<float>()) + 0.5,
                   (std::asin(position.z) / glm::pi<float>()) + 0.5);
}

}  // namespace Kazel