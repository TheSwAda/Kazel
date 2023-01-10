#include "kzpch.h"

#include "OpenGlBuffer.h"
#include <glad/glad.h>
#include "Core/Log.h"

namespace Kazel {
OpenGlVertexBuffer::OpenGlVertexBuffer(const void* vertices, uint32_t size) {
  glCreateBuffers(1, &m_ID);
  glNamedBufferStorage(m_ID, size, vertices, 0);
}

OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size) {
  glCreateBuffers(1, &m_ID);
  glNamedBufferStorage(m_ID, size, NULL, GL_DYNAMIC_STORAGE_BIT);
}

OpenGlVertexBuffer::~OpenGlVertexBuffer() { glDeleteBuffers(1, &m_ID); }

void OpenGlVertexBuffer::SetData(const void* data, uint32_t size) {
  glNamedBufferSubData(m_ID, 0, size, data);
}

void OpenGlVertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, m_ID); }

void OpenGlVertexBuffer::UnBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

const BufferLayout& OpenGlVertexBuffer::Layout() const { return m_layout; }

void OpenGlVertexBuffer::SetLayout(const BufferLayout& layout) {
  m_layout = layout;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* vertices,
                                                 uint32_t count) {
  return std::make_shared<OpenGlIndexBuffer>(vertices, count);
}

OpenGlIndexBuffer::OpenGlIndexBuffer(const uint32_t* indices, uint32_t count)
    : m_Count(count) {
  glCreateBuffers(1, &m_ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGlIndexBuffer::~OpenGlIndexBuffer() { glDeleteBuffers(1, &m_ID); }

void OpenGlIndexBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); }

void OpenGlIndexBuffer::UnBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

uint32_t OpenGlIndexBuffer::Count() { return m_Count; }
}  // namespace Kazel