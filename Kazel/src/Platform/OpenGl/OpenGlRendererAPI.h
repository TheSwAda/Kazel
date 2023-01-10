#pragma once
#include <glad/glad.h>
#include "Renderer/RenderState.h"
#include "Renderer/RendererAPI.h"

namespace Kazel {

class TypeConverterGL3x {
 public:
  static GLenum To(PrimitiveType type);
  static GLenum To(DepthTest::DepthTestFunction func);
  static GLenum To(Blending::BlendFactor factor);
  static GLenum To(Blending::BlendEquation equation);
};

class OpenGlRendererAPI : public RendererAPI {
  RenderState m_renderState;  //全局渲染状态数据
  glm::vec4 m_clearColor;
  float m_clearDepth;
  int m_clearStencil;

 public:
  void Init() override;

  void Clear(const ClearState& clearState) override;

  void CheckError() override;

  /**
 * @brief 在默认状态下绘制
 * 
 * @param vertexArray 
 * @param primitiveType 
 */
  void Draw(const Ref<VertexArray>& vertexArray,
            PrimitiveType primitiveType) override;

  void Draw(const RenderState& renderState, const Ref<VertexArray>& vertexArray,
            PrimitiveType primitiveType) override;

  void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
                            uint32_t primcount, uint32_t indexCount,
                            PrimitiveType primitiveType) override;

  void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;

 private:
  void ApplyRenderState(const RenderState& renderState);
  void ApplyDepthTest(const DepthTest& depthTest);
  void ApplyBlending(const Blending& blending);
};
}  // namespace Kazel