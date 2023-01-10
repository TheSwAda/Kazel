#include <bitset>
#include <glm/glm.hpp>

namespace Kazel {

enum class ClearBuffers {
  ColorBuffer = 1,
  DepthBuffer = 2,
  StencilBuffer = 4,
  ColorAndDepthBuffer = ColorBuffer | DepthBuffer,
  All = ColorBuffer | DepthBuffer | StencilBuffer
};

inline ClearBuffers operator&(ClearBuffers a, ClearBuffers b) {
  return static_cast<ClearBuffers>(static_cast<int>(a) & static_cast<int>(b));
}


struct ClearState {

  ClearBuffers m_buffers{ClearBuffers::All};

  std::bitset<4> m_colorMask{0xff};
  bool m_depthMask{true};
  int m_frontStencilMask{~0};
  int m_backStencilMask{~0};

  glm::vec4 m_color{0};
  float m_depth{1};
  int m_stencil{0};
};
}  // namespace Kazel
