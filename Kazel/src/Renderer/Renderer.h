#pragma once
#include "ModelLoad/Model.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace Kazel {
//TODO 简单实现，后续完善
struct Mesh {
  Ref<VertexArray> m_va;
  PrimitiveType m_primitiveType;
};

class Renderer {
 public:
  struct SceneData {
    glm::mat4 ViewProjectionMartix;
    //TODO 关于地球相关的其他数据
  };

  static void Init();
  static void ShutDown();

  static void OnWindowResize(uint32_t width, uint32_t height);
  static void BeginScence(Camera& camera);
  static void EndScence();

  static void Submit(const Ref<Shader>& shader, const Mesh& mesh,
                     const glm::mat4& tranform = glm::mat4(1.0f));

  static void SubmitInstanced(const Ref<Shader>& shader,
                              const Ref<Model>& model, uint32_t count,
                              const glm::mat4& tranform = glm::mat4(1.0f));
  static SceneData* ms_sceneData;
};
}  // namespace Kazel