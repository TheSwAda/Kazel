#include "kzpch.h"

#include "Framebuffer.h"
#include "Platform/OpenGl/OpenGlFramebuffer.h"

namespace Kazel {
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
  return CreateRef<OpenGlFramebuffer>(spec);
}
}  // namespace Kazel
