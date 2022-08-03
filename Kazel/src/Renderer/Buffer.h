#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Core/Log.h"

namespace Kazel {
enum class ShaderDataType { None = 0, Int, Float, Float2, Float3, Float4 };

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case Kazel::ShaderDataType::Int:
    case Kazel::ShaderDataType::Float:
      return 4;
    case Kazel::ShaderDataType::Float2:
      return 4 * 2;
    case Kazel::ShaderDataType::Float3:
      return 4 * 3;
    case Kazel::ShaderDataType::Float4:
      return 4 * 4;
  }
  KZ_CORE_ASSERT(false, "UnKnown Type");
  return 0;
}
struct BufferElement {
  std::string Name;
  ShaderDataType Type;
  uint32_t Size;
  uint32_t OffSet;
  uint32_t AttributeDivisor;
  bool Normalized;

  uint32_t Count() const {
    switch (Type) {
      case Kazel::ShaderDataType::Int:
      case Kazel::ShaderDataType::Float:
        return 1;
      case Kazel::ShaderDataType::Float2:
        return 2;
      case Kazel::ShaderDataType::Float3:
        return 3;
      case Kazel::ShaderDataType::Float4:
        return 4;
    }
    KZ_CORE_ASSERT(false, "UnKnown Type");
    return 0;
  }

  BufferElement(ShaderDataType type, std::string name,
                uint32_t attributeDivisor = 0, bool normalized = false)
      : Type(type),
        Name(name),
        Size(ShaderDataTypeSize(type)),
        OffSet(0),
        AttributeDivisor(attributeDivisor),
        Normalized(normalized) {}
};

class BufferLayout {
 public:
  BufferLayout() : m_Stride(0) {}

  BufferLayout(const std::initializer_list<BufferElement>& elements)
      : m_Elements(elements), m_Stride(0) {
    calculateOffsetAndStride();
  }

  uint32_t Stride() const { return m_Stride; }
  bool IsEmpty() const { return m_Elements.empty(); }

  std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
  std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
  std::vector<BufferElement>::const_iterator begin() const {
    return m_Elements.cbegin();
  }
  std::vector<BufferElement>::const_iterator end() const {
    return m_Elements.cend();
  }

 private:
  void calculateOffsetAndStride() {
    for (auto& e : m_Elements) {
      e.OffSet = m_Stride;
      m_Stride += e.Size;
    }
  }

  uint32_t m_Stride;  //²½³¤
  std::vector<BufferElement> m_Elements;
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void SetData(const void* data, uint32_t size) = 0;
  virtual void Bind() = 0;
  virtual void UnBind() = 0;

  virtual const BufferLayout& Layout() const = 0;
  virtual void SetLayout(const BufferLayout& layout) = 0;

  static Ref<VertexBuffer> Create(uint32_t size);
  static Ref<VertexBuffer> Create(const void* vertices, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;
  virtual void Bind() = 0;
  virtual void UnBind() = 0;
  virtual uint32_t Count() = 0;
  static Ref<IndexBuffer> Create(const uint32_t* vertices, uint32_t count);
};
}  // namespace Kazel