#include "kzpch.h"

#include <glm/gtc/type_ptr.hpp>
#include "OpenGlRendererAPI.h"

namespace Kazel {
GLenum TypeConverterGL3x::To(PrimitiveType type) {
  switch (type) {
    case PrimitiveType::Points:
      return GL_POINTS;
    case PrimitiveType::Lines:
      return GL_LINES;
    case PrimitiveType::LineLoop:
      return GL_LINE_LOOP;
    case PrimitiveType::LineStrip:
      return GL_LINE_STRIP;
    case PrimitiveType::Triangles:
      return GL_TRIANGLES;
    case PrimitiveType::TriangleStrip:
      return GL_TRIANGLE_STRIP;
    case PrimitiveType::LinesAdjacency:
      return GL_LINES_ADJACENCY;
    case PrimitiveType::LineStripAdjacency:
      return GL_LINE_STRIP_ADJACENCY;
    case PrimitiveType::TrianglesAdjacency:
      return GL_TRIANGLES_ADJACENCY;
    case PrimitiveType::TriangleStripAdjacency:
      return GL_TRIANGLE_STRIP_ADJACENCY;
    case PrimitiveType::TriangleFan:
      return GL_TRIANGLE_FAN;
  }
  KZ_CORE_ASSERT(false, "unknown primitive typr");
}

GLenum TypeConverterGL3x::To(DepthTest::DepthTestFunction func) {
  using enum DepthTest::DepthTestFunction;
  switch (func) {
    case Never:
      return GL_NEVER;
    case Less:
      return GL_LESS;
    case Equal:
      return GL_EQUAL;
    case LessThanOrEqual:
      return GL_LEQUAL;
    case Greater:
      return GL_GREATER;
    case NotEqual:
      return GL_NOTEQUAL;
    case GreaterThanOrEqual:
      return GL_GEQUAL;
    case Always:
      return GL_ALWAYS;
  }
  KZ_CORE_ERROR("not support depth functon");
  return GL_ALWAYS;
}

GLenum TypeConverterGL3x::To(Blending::BlendFactor factor) {
  using enum Blending::BlendFactor;
  switch (factor) {
    case Zero:
      return GL_ZERO;
    case One:
      return GL_ONE;
    case SourceColor:
      return GL_SRC_COLOR;
    case OneMinusSourceColor:
      return GL_ONE_MINUS_SRC_COLOR;
    case DestinationColor:
      return GL_DST_COLOR;
    case OneMinusDestinationColor:
      return GL_ONE_MINUS_DST_COLOR;
    case SourceAlpha:
      return GL_SRC_ALPHA;
    case OneMinusSourceAlpha:
      return GL_ONE_MINUS_SRC_ALPHA;
    case DestinationAlpha:
      return GL_DST_ALPHA;
    case OneMinusDestinationAlpha:
      return GL_ONE_MINUS_DST_ALPHA;
    case ConstantColor:
      return GL_CONSTANT_COLOR;
    case OneMinusConstantColor:
      return GL_ONE_MINUS_CONSTANT_COLOR;
    case ConstantAlpha:
      return GL_CONSTANT_ALPHA;
    case OneMinusConstantAlpha:
      return GL_ONE_MINUS_CONSTANT_ALPHA;
  }
  KZ_CORE_ERROR("not support depth factor");
  return GL_ZERO;
}

GLenum TypeConverterGL3x::To(Blending::BlendEquation equation) {
  using enum Blending::BlendEquation;
  switch (equation) {
    case Add:
      return GL_FUNC_ADD;
    case Subtract:
      return GL_FUNC_SUBTRACT;
    case ReverseSubtract:
      return GL_FUNC_REVERSE_SUBTRACT;
  }
  KZ_CORE_ERROR("not support depth equation");
  return GL_FUNC_ADD;
}

void OpenGlRendererAPI::Init() {
  //init renderer state
  if (m_renderState.m_depthTest.m_enabled) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(TypeConverterGL3x::To(m_renderState.m_depthTest.m_function));
  }

  if (m_renderState.m_programPointSize == ProgramPointSize::Enabled)
    glEnable(GL_PROGRAM_POINT_SIZE);

  if (m_renderState.m_blending.m_enabled) {
    glEnable(GL_BLEND);
    //#todo function
  }
}

void OpenGlRendererAPI::Clear(const ClearState& clearState) {

  using enum ClearBuffers;

  if (m_clearColor != clearState.m_color) {
    m_clearColor = clearState.m_color;
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
  }

  if (m_clearDepth != clearState.m_depth) {
    m_clearDepth = clearState.m_depth;
    glClearDepth(m_clearDepth);
  }

  if (m_clearStencil != clearState.m_stencil) {
    m_clearStencil = clearState.m_stencil;
    glClearStencil(m_clearStencil);
  }

  GLenum clearMask = 0;
  if ((clearState.m_buffers & ColorBuffer) == ColorBuffer) {
    clearMask |= GL_COLOR_BUFFER_BIT;
  }

  if ((clearState.m_buffers & DepthBuffer) == DepthBuffer) {
    clearMask |= GL_DEPTH_BUFFER_BIT;
  }

  if ((clearState.m_buffers & StencilBuffer) == StencilBuffer) {
    clearMask |= GL_STENCIL_BUFFER_BIT;
  }
  glClear(clearMask);
}

void OpenGlRendererAPI::CheckError() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    KZ_CORE_ERROR("OpenGl command error: 0x{0:x}", err);
  }
}

void OpenGlRendererAPI::Draw(const Ref<VertexArray>& vertexArray, PrimitiveType primitiveType) {
  auto indexBuffer = vertexArray->GetIndexBuffer();
  if (indexBuffer) {
    glDrawElements(TypeConverterGL3x::To(primitiveType), indexBuffer->Count(), GL_UNSIGNED_INT, nullptr);
  } else {
    uint32_t num = vertexArray->GetIndicesNumber();
    if (num <= 0)
      KZ_CORE_ERROR("nothing need to draw");
    glDrawArrays(TypeConverterGL3x::To(primitiveType), 0, num);
  }
}

void OpenGlRendererAPI::Draw(const RenderState& renderState, const Ref<VertexArray>& vertexArray,
                             PrimitiveType primitiveType) {
  ApplyRenderState(renderState);
  Draw(vertexArray, primitiveType);
}

void OpenGlRendererAPI::DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t primcount,
                                             uint32_t indexCount, PrimitiveType primitiveType) {
  uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->Count();
  glDrawElementsInstanced(TypeConverterGL3x::To(primitiveType), count, GL_UNSIGNED_INT, nullptr, primcount);
}

void OpenGlRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGlRendererAPI::ApplyRenderState(const RenderState& renderState) {
  ApplyDepthTest(renderState.m_depthTest);
  ApplyBlending(renderState.m_blending);
}

void OpenGlRendererAPI::ApplyDepthTest(const DepthTest& depthTest) {
  if (m_renderState.m_depthTest.m_enabled != depthTest.m_enabled) {
    depthTest.m_enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    m_renderState.m_depthTest.m_enabled = depthTest.m_enabled;
  }

  if (depthTest.m_enabled) {
    if (m_renderState.m_depthTest.m_function != depthTest.m_function) {
      glDepthFunc(TypeConverterGL3x::To(depthTest.m_function));
      m_renderState.m_depthTest.m_function = depthTest.m_function;
    }
  }
}

void OpenGlRendererAPI::ApplyBlending(const Blending& blending) {
  if (m_renderState.m_blending.m_enabled != blending.m_enabled) {
    blending.m_enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

    m_renderState.m_blending.m_enabled = blending.m_enabled;
  }

  if (blending.m_enabled) {
    if ((m_renderState.m_blending.m_sourceRGBFactor != blending.m_sourceRGBFactor) ||
        (m_renderState.m_blending.m_destinationRGBFactor != blending.m_destinationRGBFactor) ||
        (m_renderState.m_blending.m_sourceAlphaFactor != blending.m_sourceAlphaFactor) ||
        (m_renderState.m_blending.m_destinationAlphaFactor != blending.m_destinationAlphaFactor)) {
      glBlendFuncSeparate(TypeConverterGL3x::To(blending.m_sourceRGBFactor),
                          TypeConverterGL3x::To(blending.m_destinationRGBFactor),
                          TypeConverterGL3x::To(blending.m_sourceAlphaFactor),
                          TypeConverterGL3x::To(blending.m_destinationAlphaFactor));

      m_renderState.m_blending.m_sourceRGBFactor = blending.m_sourceRGBFactor;
      m_renderState.m_blending.m_destinationRGBFactor = blending.m_destinationRGBFactor;
      m_renderState.m_blending.m_sourceAlphaFactor = blending.m_sourceAlphaFactor;
      m_renderState.m_blending.m_destinationAlphaFactor = blending.m_destinationAlphaFactor;
    }

    if ((m_renderState.m_blending.m_rgbEquation != blending.m_rgbEquation) ||
        (m_renderState.m_blending.m_alphaEquation != blending.m_alphaEquation)) {
      glBlendEquationSeparate(TypeConverterGL3x::To(blending.m_rgbEquation),
                              TypeConverterGL3x::To(blending.m_alphaEquation));

      m_renderState.m_blending.m_rgbEquation = blending.m_rgbEquation;
      m_renderState.m_blending.m_alphaEquation = blending.m_alphaEquation;
    }

    if (m_renderState.m_blending.m_color != blending.m_color) {
      glBlendColor(blending.m_color[0], blending.m_color[1], blending.m_color[2], blending.m_color[3]);
      m_renderState.m_blending.m_color = blending.m_color;
    }
  }
}

void OpenGlRendererAPI::ApplyPointSize(ProgramPointSize pointSize) {
  if (m_renderState.m_programPointSize != pointSize) {
    pointSize == ProgramPointSize::Enabled ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE);
    m_renderState.m_programPointSize = pointSize;
  }
}

}  // namespace Kazel