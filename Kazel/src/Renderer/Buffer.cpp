#include "kzpch.h"

#include "Buffer.h"
#include "Platform/OpenGl/OpenGlBuffer.h"

namespace Kazel {
Kazel::Ref<Kazel::VertexBuffer> VertexBuffer::Create(uint32_t size) {
  return std::make_shared<OpenGlVertexBuffer>(size);
}

Kazel::Ref<Kazel::VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size) {
  KZ_CORE_ASSERT(vertices, "vertices is null!!");
  return std::make_shared<OpenGlVertexBuffer>(vertices, size * sizeof(float));
}

Kazel::Ref<Kazel::VertexBuffer> VertexBuffer::CreateForDouble(const double* vertices,
                                                              uint32_t count) {
  KZ_CORE_ASSERT(vertices, "vertices is null!!");
  std::vector<float> newVertices(count);

  std::transform(vertices, vertices + count, newVertices.begin(),
                 [](const double p) { return static_cast<float>(p); });

  return std::make_shared<OpenGlVertexBuffer>(newVertices.data(),
                                              newVertices.size() * sizeof(float));
}
}  // namespace Kazel