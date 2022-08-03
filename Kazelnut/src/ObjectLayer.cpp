#include "ObjectLayer.h"
#include <imgui/imgui.h>

float ObjectLayer::s_Vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

ObjectLayer::ObjectLayer(uint32_t width, uint32_t height)
    : m_CameraController(width, height) {
  m_Material.Shininess = 32;
}

void ObjectLayer::OnAttach() {
  m_Light.OnAttach();

  m_VA = Kazel::VertexArray::Create();
  const auto &VBO = Kazel::VertexBuffer::Create(s_Vertices, sizeof(s_Vertices));
  Kazel::BufferLayout layout = {
      {Kazel::ShaderDataType::Float3, "a_Pos"},
      {Kazel::ShaderDataType::Float3, "a_Normal"},
      {Kazel::ShaderDataType::Float2, "a_TexCoords"},
  };
  VBO->SetLayout(layout);
  m_VA->AddVertexBuffer(VBO);

  m_Material.m_Diffuse = Kazel::Texture2D::Create(
      R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\assert\texture\container2.png)");
  m_Material.m_Specular = Kazel::Texture2D::Create(
      R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\assert\texture\container2_specular.png)");

  m_Shader = Kazel::Shader::Create();
  m_Shader->compileShader(
      R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\assert\shader\object\object.vert)");
  m_Shader->compileShader(
      R"(D:\source\OpenGL\KazelOpenGl\KazelOpenGLnut\assert\shader\object\object.frag)");

  m_Shader->use();
  m_Shader->setUniform("u_Material.Diffuse", 0);
  m_Shader->setUniform("u_Material.Specular", 1);
  m_Shader->setUniform("u_Material.Shininess", m_Material.Shininess);

  Kazel::RenderCommand::CheckError();
}

void ObjectLayer::OnUpdate(float timeStep) {
  Kazel::Renderer::BeginScence(m_CameraController.GetCamera());
  m_CameraController.onUpdate(timeStep);

  m_Light.OnUpdate();

  m_Shader->use();
  m_Shader->setUniform("u_Material.Shininess", m_Material.Shininess);

  const auto &attribute = m_Light.GetLightAttribute();
  m_Shader->setUniform("u_Light.Ambient", attribute.Ambient);
  m_Shader->setUniform("u_Light.Diffuse", attribute.Diffuse);
  m_Shader->setUniform("u_Light.Specular", attribute.Specular);
  m_Shader->setUniform("u_Light.Position", attribute.Position);

  m_Shader->setUniform("u_CameraPos",
                       m_CameraController.GetCamera().GetPosition());
  glm::mat4 model = glm::rotate(m_Angle, glm::vec3(0, 1.0f, 0));
  m_Shader->setUniform(
      "u_NomalMatrix",
      glm::mat3(glm::transpose(glm::inverse(model))));  //·¨Ïß¾ØÕó

  m_Material.m_Diffuse->Bind(0);
  m_Material.m_Specular->Bind(1);
  Kazel::Renderer::Submit(m_Shader, m_VA, model, 36);

  Kazel::Renderer::EndScence();
}

void ObjectLayer::OnImGuiRender() {
  KZ_PROFILE_FUNCTION();
  ImGui::Begin("setting");
  m_Light.onImGuiRender();
  ImGui::Text("Object");
  ImGui::SliderAngle("angle", &m_Angle);
  ImGui::SliderInt("Object Shininess", &m_Material.Shininess, 1, 400);
  ImGui::Text("Info:");

  ImGui::End();
}

void ObjectLayer::OnEvent(Kazel::Event &e) { m_CameraController.onEvent(e); }