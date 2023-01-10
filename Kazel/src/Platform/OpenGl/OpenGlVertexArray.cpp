#include "kzpch.h"
#include "OpenGlVertexArray.h"

#include <glad/glad.h>

namespace Kazel {
static GLenum ShaderDataTypeToGlBase(ShaderDataType type) {
  switch (type) {
    case Kazel::ShaderDataType::Int:
      return GL_INT;
    case Kazel::ShaderDataType::Float:
      return GL_FLOAT;
    case Kazel::ShaderDataType::Float2:
      return GL_FLOAT;
    case Kazel::ShaderDataType::Float3:
      return GL_FLOAT;
    case Kazel::ShaderDataType::Float4:
      return GL_FLOAT;
  }
  KZ_CORE_ASSERT(false, "UnKnown Type");
  return 0;
}

OpenGlVertexArray::OpenGlVertexArray() {
  glCreateVertexArrays(1, &m_ID);
}

OpenGlVertexArray::~OpenGlVertexArray() {
  glDeleteVertexArrays(1, &m_ID);
}

void OpenGlVertexArray::Bind() {
  glBindVertexArray(m_ID);
}

void OpenGlVertexArray::UnBind() {
  glBindVertexArray(0);
}

void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
  auto& layout = vertexBuffer->Layout();
  if (layout.IsEmpty()) {
    KZ_CORE_ERROR("VB is empty");
  }

  glBindVertexArray(m_ID);
  vertexBuffer->Bind();

  for (const auto& e : layout) {

    // 顶点属性,顶点属性的大小,数据类型,标准化,步长(连续的顶点属性组之间的间隔),偏移量(Offset)
    glVertexAttribPointer(m_Index, e.Count(), ShaderDataTypeToGlBase(e.Type),
                          e.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer->Layout().Stride(),
                          (const void*)(e.OffSet));

    glVertexAttribDivisor(m_Index, e.AttributeDivisor);
    glEnableVertexAttribArray(m_Index++);
  }
  m_Arrayes.push_back(vertexBuffer);
  glBindVertexArray(0);
}

void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) {
  glBindVertexArray(m_ID);
  IndexBuffer->Bind();

  m_IndexBuffer = IndexBuffer;
  glBindVertexArray(0);
}
}  // namespace Kazel