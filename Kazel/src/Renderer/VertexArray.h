#pragma once
#include <memory>
#include "Buffer.h"

namespace Kazel {
class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void Bind() = 0;
  virtual void UnBind() = 0;
  virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
  virtual void SetIndexBuffer(const Ref<IndexBuffer> &IndexBuffer) = 0;
  virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;
  static Ref<VertexArray> Create();
};

}  // namespace Kazel