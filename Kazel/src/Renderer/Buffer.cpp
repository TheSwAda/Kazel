#include "kzpch.h"

#include "Buffer.h"
#include "Platform/OpenGl/OpenGlBuffer.h"

namespace Kazel {
Kazel::Ref<Kazel::VertexBuffer> VertexBuffer::Create(uint32_t size) {
  return std::make_shared<OpenGlVertexBuffer>(size);
}

Kazel::Ref<Kazel::VertexBuffer> VertexBuffer::Create(const void* vertices,
                                                     uint32_t size) {
  KZ_CORE_ASSERT(vertices, "vertices is null!!");
  return std::make_shared<OpenGlVertexBuffer>(vertices, size);
}
}  // namespace Kazel