#pragma once
#include "RenderCommand.h"

#include "ModelLoad/Model.h"
#include "Camera.h"
#include "Shader.h"

namespace Kazel {
class Renderer {
 public:
  struct SceneData {
    glm::mat4 ViewProjectionMartix;
  };

  static void Init();
  static void ShutDown();

  static void OnWindowResize(uint32_t width, uint32_t height);
  static void BeginScence(Camera& camera);
  static void EndScence();

  static void Submit(const Ref<Shader>& shader,
                     const Ref<VertexArray>& vertexArray,
                     const glm::mat4& tranform = glm::mat4(1.0f),
                     uint32_t vertexCount = 0);

  static void Submit(const Ref<Shader>& shader, const Ref<Model>& model,
                     const glm::mat4& tranform = glm::mat4(1.0f));

  static void SubmitInstanced(const Ref<Shader>& shader,
                              const Ref<Model>& model, uint32_t count,
                              const glm::mat4& tranform = glm::mat4(1.0f));
  static SceneData* s_SceneData;
};
}  // namespace Kazel