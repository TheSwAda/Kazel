#pragma once
#include "Renderer/VertexArray.h"

namespace Kazel {
class OpenGlVertexArray : public VertexArray {
 public:
  OpenGlVertexArray();
  virtual ~OpenGlVertexArray();

  void Bind() override;
  void UnBind() override;
  void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
  void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) override;

  Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

 private:
  std::vector<Ref<VertexBuffer>> m_Arrayes;
  Ref<IndexBuffer> m_IndexBuffer;

  uint32_t m_Index = 0;
  uint32_t m_ID;
};
}  // namespace Kazel
