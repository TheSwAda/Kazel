#include "kzpch.h"

#include "VertexArray.h"
#include "Platform/OpenGl/OpenGlVertexArray.h"

namespace Kazel {
Ref<VertexArray> VertexArray::Create() {
  return std::make_shared<OpenGlVertexArray>();
}
}  // namespace Kazel