#pragma once
#include <glm/glm.hpp>
namespace Kazel {

/// @brief 混合 https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/03%20Blending/
struct Blending {
  /// @brief 混合因子
  enum class BlendFactor {
    Zero,
    One,
    SourceColor,
    OneMinusSourceColor,
    SourceAlpha,
    OneMinusSourceAlpha,
    DestinationAlpha,
    OneMinusDestinationAlpha,
    DestinationColor,
    OneMinusDestinationColor,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha
  };

  /// @brief 混合方程
  enum class BlendEquation { Add, Subtract, ReverseSubtract };

  bool m_enabled{false};
  BlendFactor m_sourceRGBFactor{BlendFactor::One};          //源颜色因子
  BlendFactor m_sourceAlphaFactor{BlendFactor::One};        //源alpha通道因子
  BlendFactor m_destinationRGBFactor{BlendFactor::Zero};    //目的颜色因子
  BlendFactor m_destinationAlphaFactor{BlendFactor::Zero};  //目的alpha因子
  BlendEquation m_rgbEquation{BlendEquation::Add};          //颜色运算符
  BlendEquation m_alphaEquation{BlendEquation::Add};        //alpha运算符
  glm::u8vec4 m_color{0, 0, 0, 0};                          //constant颜色参数
};

/// @brief 深度测试
struct DepthTest {
  enum class DepthTestFunction {
    Never,  //(永远通过深度测试)
    Less,   //最后绘制的片段将会总是会渲染在之前绘制片段的上面
    Equal,
    LessThanOrEqual,
    Greater,
    NotEqual,
    GreaterThanOrEqual,
    Always
  };

  bool m_enabled{true};
  DepthTestFunction m_function{DepthTestFunction::Less};
};

/**
 * @brief gl_PointSize
*/
enum class ProgramPointSize { Enabled, Disabled };

struct RenderState {
  DepthTest m_depthTest;
  Blending m_blending;
  ProgramPointSize m_programPointSize{ProgramPointSize::Enabled};
};
//TODO complete other render state
}  // namespace Kazel