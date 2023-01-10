#include "kzpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <numeric>

#include "Platform/OpenGl/OpenGlShader.h"
#include "RenderCommand.h"

namespace Kazel {
struct QuadVertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoord;
  float TexIndex;
};

struct Renderer2DData {
  static const uint32_t MaxQuads = 20000;
  static const uint32_t MaxVertices = MaxQuads * 4;
  static const uint32_t MaxIndices = MaxQuads * 6;
  static constexpr uint32_t MaxTextureSlots = 32;  // todo from renderer

  Ref<VertexArray> QuadVertexArray;
  Ref<VertexBuffer> QuadVertexBuffer;
  uint32_t QuadIndexCount = 0;
  Ref<Shader> QuadShader;

  std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
  Ref<Texture2D> whiteTexture;
  uint32_t TextureSlotIndex = 0;  // starting at 0, 0 is white texture

  glm::vec4 QuadVertexPosition[4];
  QuadVertex* QuadVertexBufferBase = nullptr;
  QuadVertex* QuadVertexBufferPtr = nullptr;
};

static Renderer2DData s_2DData;

void Renderer2D::Init() {
  s_2DData.QuadShader = Shader::Create();
  s_2DData.QuadVertexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(QuadVertex));
  s_2DData.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Pos"},
                                        {ShaderDataType::Float4, "a_Color"},
                                        {ShaderDataType::Float2, "a_TexCoord"},
                                        {ShaderDataType::Float, "a_TexIndex"}});

  s_2DData.QuadVertexBufferBase = new QuadVertex[s_2DData.MaxVertices];

  // shader
  s_2DData.QuadShader->compileShaderSource(s_2DVertexShader, ShaderType::ST_Vertex);
  s_2DData.QuadShader->compileShaderSource(s_2DFrageShader, ShaderType::ST_Fragment);

  std::array<int32_t, Renderer2DData::MaxTextureSlots> samplers;
  std::iota(samplers.begin(), samplers.end(), 0);
  s_2DData.QuadShader->setUniform("u_Textures", samplers.data(), samplers.size());

  // textures
  s_2DData.whiteTexture = Texture2D::Create(1, 1, 4);
  uint32_t whiteTextureData = 0xffffffff;
  s_2DData.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

  s_2DData.TextureSlots[0] = s_2DData.whiteTexture;
  s_2DData.TextureSlotIndex = 1;

  s_2DData.QuadVertexPosition[0] = glm::vec4{-0.5f, -0.5f, 0, 1.0f};
  s_2DData.QuadVertexPosition[1] = glm::vec4{0.5f, -0.5f, 0, 1.0f};
  s_2DData.QuadVertexPosition[2] = glm::vec4{0.5f, 0.5f, 0, 1.0f};
  s_2DData.QuadVertexPosition[3] = glm::vec4{-0.5f, 0.5f, 0, 1.0f};

  s_2DData.QuadVertexArray = VertexArray::Create();
  s_2DData.QuadVertexArray->AddVertexBuffer(s_2DData.QuadVertexBuffer);
  s_2DData.QuadVertexArray->SetIndexBuffer(GenIndexBuffer());
}

void Renderer2D::ShutDown() {
  delete[] s_2DData.QuadVertexBufferBase;
}

void Renderer2D::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer2D::BeginScence(OrthographicCamera& camera) {
  s_2DData.QuadShader->use();
  s_2DData.QuadShader->setUniform("u_ViewProjection", camera.ViewPorjection());
  StartBatch();
}

void Renderer2D::EndScence() {
  if (s_2DData.QuadIndexCount == 0)
    return;  // Nothing to draw

  uint32_t dataSize =
      (uint32_t)((uint8_t*)s_2DData.QuadVertexBufferPtr - (uint8_t*)s_2DData.QuadVertexBufferBase);
  s_2DData.QuadVertexBuffer->SetData(s_2DData.QuadVertexBufferBase, dataSize);

  // bind texture
  for (uint32_t i = 0; i < s_2DData.TextureSlotIndex; ++i) {
    s_2DData.TextureSlots[i]->Bind(i);
  }

  s_2DData.QuadVertexArray->SetIndicesNumber(s_2DData.QuadIndexCount);
  s_2DData.QuadVertexArray->Bind();
  RenderCommand::Draw(s_2DData.QuadVertexArray, PrimitiveType::Triangles);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color) {
  glm::mat4 transform = glm::translate(pos) *
                        glm::rotate(glm::radians(rotation), glm::vec3{0, 0, 1.0f}) *
                        glm::scale(glm::vec3{size, 1.0f});

  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation,
                          const Ref<Texture2D>& texture) {
  glm::mat4 transform = glm::translate(pos) *
                        glm::rotate(glm::radians(rotation), glm::vec3{0, 0, 1.0f}) *
                        glm::scale(glm::vec3{size, 1.0f});
  DrawQuad(transform, texture);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, glm::vec4 color) {
  int textureIndex = 0;

  constexpr size_t quadVertexCount = 4;
  constexpr glm::vec2 quadTexCoord[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices) {
    NextBatch();
  }

  for (size_t i = 0; i < quadVertexCount; ++i) {
    s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPosition[i];
    s_2DData.QuadVertexBufferPtr->Color = color;
    s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_2DData.QuadVertexBufferPtr->TexCoord = quadTexCoord[i];

    s_2DData.QuadVertexBufferPtr++;
  }
  s_2DData.QuadIndexCount += 6;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture) {
  constexpr glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

  if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices) {
    NextBatch();
  }

  auto it = std::find(s_2DData.TextureSlots.begin(), s_2DData.TextureSlots.end(), texture);

  if (it == s_2DData.TextureSlots.end()) {
    s_2DData.TextureSlots[s_2DData.TextureSlotIndex] = texture;
    it = s_2DData.TextureSlots.begin() + s_2DData.TextureSlotIndex;
    s_2DData.TextureSlotIndex++;
  }

  int textureIndex = std::distance(s_2DData.TextureSlots.begin(), it);

  constexpr size_t quadVertexCount = 4;
  constexpr glm::vec2 quadTexCoord[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  for (size_t i = 0; i < quadVertexCount; ++i) {
    s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPosition[i];
    s_2DData.QuadVertexBufferPtr->Color = color;
    s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_2DData.QuadVertexBufferPtr->TexCoord = quadTexCoord[i];

    s_2DData.QuadVertexBufferPtr++;
  }
  s_2DData.QuadIndexCount += 6;
}

Kazel::Ref<Kazel::IndexBuffer> Renderer2D::GenIndexBuffer() {
  uint32_t* quadIndices = new uint32_t[s_2DData.MaxIndices];
  uint32_t offset = 0;
  for (uint32_t i = 0; i < s_2DData.MaxIndices; i += 6) {
    quadIndices[i + 0] = offset + 0;
    quadIndices[i + 1] = offset + 1;
    quadIndices[i + 2] = offset + 3;

    quadIndices[i + 3] = offset + 1;
    quadIndices[i + 4] = offset + 2;
    quadIndices[i + 5] = offset + 3;

    offset += 4;
  }

  Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_2DData.MaxIndices);

  delete[] quadIndices;
  return quadIB;
}

void Renderer2D::StartBatch() {
  s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;
  s_2DData.QuadIndexCount = 0;
  s_2DData.TextureSlotIndex = 1;
}

void Renderer2D::NextBatch() {
  EndScence();
  StartBatch();
}

const char* Renderer2D::s_2DVertexShader = R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main()
{
    v_TexIndex = a_TexIndex;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}
)";

const char* Renderer2D::s_2DFrageShader = R"(
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main() {
  color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}
)";
}  // namespace Kazel