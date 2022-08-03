#pragma once
#include "Renderer/Texture.h"
#include <glad/glad.h>

namespace Kazel {
class OpenGlTexture2D : public Texture2D {
 public:
  OpenGlTexture2D(const std::string& path);
  OpenGlTexture2D(uint32_t width, uint32_t height, uint32_t channel);

  virtual ~OpenGlTexture2D();

  uint32_t GetWidth() const override { return m_Width; }

  uint32_t GetHeight() const override { return m_Height; }

  void Bind(uint32_t slot = 0) const override;

  void UnBind() const override;

  void SetData(const void* dataC, uint32_t size) const override;

  void SetWrapping(EAxis axis, EWrapType type,
                   const float* color = nullptr) override;

  void SetFilter(EFilterType type, EFilterOperator op) override;

  bool operator==(const Texture&) const override;

 private:
  void InitTexture(int width, int height, int component);

 private:
  std::string m_Path;
  uint32_t m_Width, m_Height, m_Channels;
  GLenum m_DataFormat;

  uint32_t m_ID;
};
}  // namespace Kazel