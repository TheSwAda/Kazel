#pragma once
#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Kazel {
class RendererAPI {
 public:
  virtual void Init() = 0;
  virtual void ClearColor(const glm::vec4& color) = 0;
  virtual void SetColor(const glm::vec4& color) = 0;
  virtual void Clear() = 0;
  virtual void CheckError() = 0;

  virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;

  virtual void Draw(const Ref<VertexArray>& vertexArray,
                    uint32_t indexCount = 0) = 0;

  virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
                                    uint32_t count,
                                    uint32_t indexCount = 0) = 0;
};
}  // namespace Kazel