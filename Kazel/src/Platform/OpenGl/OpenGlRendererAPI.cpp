#include "kzpch.h"

#include "OpenGlRendererAPI.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Kazel {
void OpenGlRendererAPI::Init() {
  //启用混合
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // glEnable(GL_CULL_FACE);
  // glEnable(GL_STENCIL_TEST);

  glEnable(GL_DEPTH_TEST);
  /**
   * 修改深度测试中使用的比较运算符
   * GL_ALWAYS	(永远通过深度测试)
   * 最后绘制的片段将会总是会渲染在之前绘制片段的上面 GL_LESS
   * （默认）深度值小，优先绘制
   */

  //深度值的范围在0到1之间，需要将near和far之间的z值转换到这一范围
  /**
   * 防止深度冲突
   * 1.永远不要把多个物体摆得太靠近，以至于它们的一些三角形会重叠
   */
  // glDepthFunc(GL_ALWAYS);

  /**
   * 模板测试
   */
}

void OpenGlRendererAPI::ClearColor(const glm::vec4& color) {
  glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
}

void OpenGlRendererAPI::SetColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGlRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGlRendererAPI::CheckError() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    KZ_CORE_ERROR("OpenGl command error: 0x{0:x}", err);
  }
}

void OpenGlRendererAPI::Draw(const Ref<VertexArray>& vertexArray,
                             uint32_t indexCount /*= 0*/) {
  uint32_t count = indexCount;
  if (vertexArray->GetIndexBuffer()) {
    count = vertexArray->GetIndexBuffer()->Count();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, count);
  }
}

void OpenGlRendererAPI::DrawIndexedInstanced(
    const Ref<VertexArray>& vertexArray, uint32_t primcount,
    uint32_t indexCount /*= 0*/) {
  uint32_t count =
      indexCount ? indexCount : vertexArray->GetIndexBuffer()->Count();
  glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr,
                          primcount);
}

void OpenGlRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}
}  // namespace Kazel