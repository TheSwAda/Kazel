#pragma once
#include "Renderer/RendererAPI.h"

namespace Kazel {
class OpenGlRendererAPI : public RendererAPI {
 public:
  void Init() override;

  void ClearColor(const glm::vec4& color) override;

  void SetColor(const glm::vec4& color) override;

  void Clear() override;

  void CheckError() override;

  void Draw(const Ref<VertexArray>& vertexArray,
            uint32_t vertexCount = 0) override;

  void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
                            uint32_t primcount,
                            uint32_t indexCount = 0) override;

  void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;
};
}  // namespace Kazel