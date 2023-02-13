#pragma once

#include "RendererAPI.h"

namespace Kazel {

class RenderCommand {
 public:
  static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    m_RenderAPI->SetViewPort(x, y, width, height);
  }

  static void Init() { m_RenderAPI->Init(); }
  static void Clear(const ClearState& clearState) { m_RenderAPI->Clear(clearState); }
  static void CheckError() { m_RenderAPI->CheckError(); }

  static void Draw(const Ref<VertexArray>& vertexArray, PrimitiveType type) { m_RenderAPI->Draw(vertexArray, type); }

  static void Draw(const RenderState& renderState, const Ref<VertexArray>& vertexArray, PrimitiveType type) {
    m_RenderAPI->Draw(renderState, vertexArray, type);
  }

  static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t primcount, uint32_t indexCount = 0) {
    //TODO need to fix type
    m_RenderAPI->DrawIndexedInstanced(vertexArray, primcount, indexCount, PrimitiveType::Triangles);
  }

 private:
  static RendererAPI* m_RenderAPI;
};
}  // namespace Kazel