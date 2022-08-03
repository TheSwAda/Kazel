#include "kzpch.h"

#include "Texture.h"
#include "Platform/OpenGl/OpenGlTexture.h"

namespace Kazel {
Ref<Texture2D> Texture2D::Create(const std::string& path) {
  return CreateRef<OpenGlTexture2D>(path);
}

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height,
                                 uint32_t channels) {
  return CreateRef<OpenGlTexture2D>(width, height, channels);
}
}  // namespace Kazel