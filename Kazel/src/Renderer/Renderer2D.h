#pragma once
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Kazel {
class Renderer2D {
 public:
  static void Init();
  static void ShutDown();
  static void OnWindowResize(uint32_t width, uint32_t height);
  static void BeginScence(OrthographicCamera& camera);
  static void EndScence();

  static void DrawQuad(const glm::mat4& transform, glm::vec4 color);

  static void DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation,
                       glm::vec4 color);
  inline static void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation,
                              glm::vec4 color) {
    DrawQuad(glm::vec3(pos, 0.0f), size, rotation, color);
  }
  inline static void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {
    DrawQuad(glm::vec3(pos, 0.0f), size, 0.0f, color);
  }
  inline static void DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) {
    DrawQuad(pos, size, 0.0f, color);
  }

  static void DrawQuad(const glm::mat4& transform,
                       const Ref<Texture2D>& texture);

  static void DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation,
                       const Ref<Texture2D>& texture);

  inline static void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation,
                              const Ref<Texture2D>& texture) {
    DrawQuad(glm::vec3(pos, 0.0f), size, rotation, texture);
  }

  inline static void DrawQuad(glm::vec2 pos, glm::vec2 size,
                              const Ref<Texture2D>& texture) {
    DrawQuad(glm::vec3(pos, 0.0f), size, 0, texture);
  }

  inline static void DrawQuad(glm::vec3 pos, glm::vec2 size,
                              const Ref<Texture2D>& texture) {
    DrawQuad(pos, size, 0, texture);
  }

 private:
  static Ref<IndexBuffer> GenIndexBuffer();

  static void StartBatch();
  static void NextBatch();

  struct Statictics
  {

  };
 private:
  static const char* s_2DVertexShader;
  static const char* s_2DFrageShader;
};
}  // namespace Kazel