#include "kzpch.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGlShader.h"
#include "Renderer2D.h"

namespace Kazel {
Renderer::SceneData* Renderer::ms_sceneData = nullptr;

void Renderer::Init() {
  ms_sceneData = new SceneData;
  RenderCommand::Init();
  //Renderer2D::Init();
}

void Renderer::ShutDown() {
  if (ms_sceneData) {
    delete ms_sceneData;
    ms_sceneData = nullptr;
  }
  //Renderer2D::ShutDown();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewPort(0, 0, width, height);
}

void Renderer::BeginScence(Camera& camera) {
  ms_sceneData->ViewProjectionMartix = camera.ViewPorjection();
}

void Renderer::EndScence() {}

void Renderer::Submit(const Ref<Shader>& shader, const Mesh& mesh,
                      const glm::mat4& tranform /*= glm::mat4(1.0f)*/) {
  shader->use();
  shader->setUniform("u_ViewProjection", ms_sceneData->ViewProjectionMartix);
  shader->setUniform("u_Transform", tranform);

  mesh.m_va->Bind();
  RenderCommand::Draw(mesh.m_va, mesh.m_primitiveType);
}

void Renderer::SubmitInstanced(const Ref<Shader>& shader, const Ref<Model>& model, uint32_t count,
                               const glm::mat4& tranform /*= glm::mat4(1.0f)*/) {
  shader->use();
  shader->setUniform("u_ViewProjection", ms_sceneData->ViewProjectionMartix);
  shader->setUniform("u_Transform", tranform);

  model->DrawInstanced(shader, count);
}
}  // namespace Kazel