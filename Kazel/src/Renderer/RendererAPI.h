#pragma once
#include <glm/glm.hpp>
#include "Renderer/VertexArray.h"
#include "Renderer/RenderState.h"
#include "Renderer/ClearState.h"

namespace Kazel {

enum class PrimitiveType {
  Points,
  Lines,
  LineLoop,
  LineStrip,
  Triangles,
  TriangleStrip,
  TriangleFan,
  LinesAdjacency,
  LineStripAdjacency,
  TrianglesAdjacency,
  TriangleStripAdjacency
};

class RendererAPI {
 public:
  virtual void Init() = 0;
  virtual void Clear(const ClearState& clearState) = 0;
  virtual void CheckError() = 0;

  virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;

  virtual void Draw(const Ref<VertexArray>& vertexArray,
                    PrimitiveType primitiveType) = 0;

  virtual void Draw(const RenderState& renderState,
                    const Ref<VertexArray>& vertexArray,
                    PrimitiveType primitiveType) = 0;

  virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
                                    uint32_t count, uint32_t indexCount,
                                    PrimitiveType primitiveType) = 0;
};
}  // namespace Kazel