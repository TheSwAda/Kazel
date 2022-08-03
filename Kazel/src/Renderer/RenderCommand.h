#pragma once

#include "RendererAPI.h"

namespace Kazel {
class RenderCommand {
 public:
  static void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                          uint32_t height) {
    m_RenderAPI->SetViewPort(x, y, width, height);
  }
  static void Init() { m_RenderAPI->Init(); }
  static void ClearColor(const glm::vec4& color) {
    m_RenderAPI->ClearColor(color);
  }

  static void SetColor(const glm::vec4& color) { m_RenderAPI->SetColor(color); }
  static void Clear() { m_RenderAPI->Clear(); }
  static void CheckError() { m_RenderAPI->CheckError(); }

  static void Draw(const Ref<VertexArray>& vertexArray,
                   uint32_t vertexCount = 0) {
    m_RenderAPI->Draw(vertexArray, vertexCount);
  }

  static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
                                   uint32_t primcount,
                                   uint32_t indexCount = 0) {
    m_RenderAPI->DrawIndexedInstanced(vertexArray, primcount, indexCount);
  }

 private:
  static RendererAPI* m_RenderAPI;
};
}  // namespace Kazel