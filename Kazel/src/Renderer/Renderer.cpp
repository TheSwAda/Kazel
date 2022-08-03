#include "kzpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGl/OpenGlShader.h"

namespace Kazel {
Renderer::SceneData* Renderer::s_SceneData = nullptr;

void Renderer::Init() {
  s_SceneData = new SceneData;
  RenderCommand::Init();
  Renderer2D::Init();
}

void Renderer::ShutDown() {
  if (s_SceneData) {
    delete s_SceneData;
    s_SceneData = nullptr;
  }
  Renderer2D::ShutDown();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewPort(0, 0, width, height);
}

void Renderer::BeginScence(Camera& camera) {
  s_SceneData->ViewProjectionMartix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScence() {}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertexArray,
                      const glm::mat4& tranform /*= glm::mat4(1.0f)*/,
                      uint32_t vertexCount /*= 0*/) {
  shader->use();
  shader->setUniform("u_ViewProjection", s_SceneData->ViewProjectionMartix);
  shader->setUniform("u_Transform", tranform);

  vertexArray->Bind();
  if (vertexCount == 0)
    RenderCommand::Draw(vertexArray);
  else
    RenderCommand::Draw(vertexArray, vertexCount);
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<Model>& model,
                      const glm::mat4& tranform /*= glm::mat4(1.0f)*/) {
  shader->use();
  shader->setUniform("u_ViewProjection", s_SceneData->ViewProjectionMartix);
  shader->setUniform("u_Transform", tranform);

  model->Draw(shader);
}

void Renderer::SubmitInstanced(
    const Ref<Shader>& shader, const Ref<Model>& model, uint32_t count,
    const glm::mat4& tranform /*= glm::mat4(1.0f)*/) {
  shader->use();
  shader->setUniform("u_ViewProjection", s_SceneData->ViewProjectionMartix);
  shader->setUniform("u_Transform", tranform);

  model->DrawInstanced(shader, count);
}
}  // namespace Kazel