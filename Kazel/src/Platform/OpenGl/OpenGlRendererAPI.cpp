#include "kzpch.h"

#include "OpenGlRendererAPI.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Kazel {
void OpenGlRendererAPI::Init() {
  //���û��
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // glEnable(GL_CULL_FACE);
  // glEnable(GL_STENCIL_TEST);

  glEnable(GL_DEPTH_TEST);
  /**
   * �޸���Ȳ�����ʹ�õıȽ������
   * GL_ALWAYS	(��Զͨ����Ȳ���)
   * �����Ƶ�Ƭ�ν������ǻ���Ⱦ��֮ǰ����Ƭ�ε����� GL_LESS
   * ��Ĭ�ϣ����ֵС�����Ȼ���
   */

  //���ֵ�ķ�Χ��0��1֮�䣬��Ҫ��near��far֮���zֵת������һ��Χ
  /**
   * ��ֹ��ȳ�ͻ
   * 1.��Զ��Ҫ�Ѷ������ڵ�̫���������������ǵ�һЩ�����λ��ص�
   */
  // glDepthFunc(GL_ALWAYS);

  /**
   * ģ�����
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