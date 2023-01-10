#pragma once

#include "Renderer/Buffer.h"

namespace Kazel {
class OpenGlVertexBuffer : public VertexBuffer {
 public:
  OpenGlVertexBuffer(uint32_t size);
  OpenGlVertexBuffer(const void* vertices, uint32_t size);
  virtual ~OpenGlVertexBuffer();

  void SetData(const void* data, uint32_t size) override;
  void Bind() override;
  void UnBind() override;

  const BufferLayout& Layout() const override;
  void SetLayout(const BufferLayout& layout) override;

 private:
  uint32_t m_ID;
  BufferLayout m_layout;
};

class OpenGlIndexBuffer : public IndexBuffer {
 public:
  OpenGlIndexBuffer(const uint32_t* indices, uint32_t count);
  virtual ~OpenGlIndexBuffer();
  void Bind() override;
  void UnBind() override;

  uint32_t Count() override;

 private:
  uint32_t m_ID;
  uint32_t m_Count;
};
}  // namespace Kazel
