#include "Light.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

float CLight::s_Vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

CLight::CLight() : m_LightAttribute({1.2f, 0.2f, 0.5f}) {}

void CLight::OnAttach() {
  m_VA = Kazel::VertexArray::Create();
  const auto& VBO = Kazel::VertexBuffer::Create(s_Vertices, sizeof(s_Vertices));
  Kazel::BufferLayout layout = {{Kazel::ShaderDataType::Float3, "a_Pos"},
                                {Kazel::ShaderDataType::Float2, "a_TexCoords"}};
  VBO->SetLayout(layout);
  m_VA->AddVertexBuffer(VBO);

  m_Shader = Kazel::Shader::Create();
  m_Shader->compileShader(R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\src\Light\light.vert)");
  m_Shader->compileShader(R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\src\Light\light.frag)");
  m_Shader->use();
}

void CLight::OnUpdate() {
  glm::mat4 model =
      glm::translate(m_LightAttribute.Position) * glm::scale(glm::vec3(0.2f));
  Kazel::Renderer::Submit(m_Shader, m_VA, model, 36);
}

void CLight::onImGuiRender() {
  ImGui::Text("Light");
  ImGui::SliderFloat3("Light pos", glm::value_ptr(m_LightAttribute.Position),
                      -5.0f, 5.0f);
  ImGui::ColorEdit3("Light Ambient", glm::value_ptr(m_LightAttribute.Ambient));
  ImGui::ColorEdit3("Light Diffuse", glm::value_ptr(m_LightAttribute.Diffuse));
  ImGui::ColorEdit3("Light Specular",
                    glm::value_ptr(m_LightAttribute.Specular));
}