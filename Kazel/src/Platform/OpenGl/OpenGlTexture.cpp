#include "kzpch.h"

#include "OpenGlTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stdImage/stb_image.h>

#include "Core/Log.h"

namespace Kazel {
OpenGlTexture2D::OpenGlTexture2D(const std::string& path)
    : m_Width(0), m_Height(0), m_ID(0), m_Path(path) {
  stbi_set_flip_vertically_on_load(false);
  int width, height, nrComponents;
  unsigned char* data =
      stbi_load(m_Path.c_str(), &width, &height, &nrComponents, 0);
  KZ_CORE_ASSERT(data, "failed to load {0} for texture", m_Path);

  InitTexture(width, height, nrComponents);

  glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat,
                      GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);
}

OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height,
                                 uint32_t channels)
    : m_ID(0) {
  InitTexture(width, height, channels);
}

OpenGlTexture2D::~OpenGlTexture2D() { glDeleteTextures(1, &m_ID); }

void OpenGlTexture2D::Bind(uint32_t slot) const {
  glBindTextureUnit(slot, m_ID);
}

void OpenGlTexture2D::UnBind() const { glBindTexture(GL_TEXTURE_2D, 0); }

void OpenGlTexture2D::SetData(const void* dataC, uint32_t size) const {
  KZ_CORE_ASSERT(size == m_Width * m_Height * m_Channels,
                 "data must be entire texture!");
  glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat,
                      GL_UNSIGNED_BYTE, dataC);
}

void OpenGlTexture2D::SetWrapping(EAxis axis, EWrapType type,
                                  const float* color /* = nullptr*/) {
  GLenum name;
  switch (axis) {
    case EWrap_S:
      name = GL_TEXTURE_WRAP_S;
      break;
    case EWrap_T:
    default:
      name = GL_TEXTURE_WRAP_T;
      break;
  }

  GLint parameter;
  switch (type) {
    case EWrap_Mirror_Repeat:
      parameter = GL_MIRRORED_REPEAT;
      break;
    case EWrap_Clap_To_Edge:
      parameter = GL_CLAMP_TO_EDGE;
      break;
    case EWrap_Clap_To_Border:
      parameter = GL_CLAMP_TO_BORDER;
      break;
    case EWrap_Repeat:
    default:
      parameter = GL_REPEAT;
      break;
  }
  glTextureParameteri(m_ID, name, parameter);
  if (type == EWrap_Clap_To_Border) {
    glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, color);
  }
}

void OpenGlTexture2D::SetFilter(EFilterType type, EFilterOperator op) {
  GLenum name;
  switch (type) {
    case EFilter_Min:
      name = GL_TEXTURE_MIN_FILTER;
      break;
    case EFilter_Max:
    default:
      name = GL_TEXTURE_MAG_FILTER;
      break;
  }

  GLint parameter = GL_LINEAR;
  switch (op) {
    case EFilter_Nearest:
      parameter = GL_NEAREST;
      break;
    case EFilter_Linear:
      parameter = GL_LINEAR;
      break;
  }
  glTextureParameteri(m_ID, name, parameter);
}

bool OpenGlTexture2D::operator==(const Texture& other) const {
  return m_ID == dynamic_cast<const OpenGlTexture2D&>(other).m_ID;
}

void OpenGlTexture2D::InitTexture(int width, int height, int component) {
  m_Width = width;
  m_Height = height;
  m_Channels = component;

  GLenum internalFormat = 0;
  if (m_Channels == 1) {
    internalFormat = GL_RED_INTEGER;
    m_DataFormat = GL_RED;
  } else if (m_Channels == 3) {
    internalFormat = GL_RGB8;
    m_DataFormat = GL_RGB;
  } else if (m_Channels == 4) {
    internalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
  } else {
    KZ_CORE_ASSERT(false, "picture format is unknown");
  }

  glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
  glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);

  /**
   * 纹理过滤(根据纹理坐标,提取像素的颜色)
   * GL_NEAREST  选择中心点最接近纹理坐标的像素
   * GL_LINEAR   基于纹理坐标附近的纹理像素，计算出一个插值
   */
  glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
}  // namespace Kazel