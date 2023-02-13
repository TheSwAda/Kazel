#include "TestLayer.h"
#include <imgui/imgui.h>
#include <string>
float TestLayer::s_Vertices[] = {
    0,     0,     0,     // left
    0,     0.5,   0,  // right
    0,     0,     0.5f,  // top
    -0.5f, -0.5f, 0.0f,  // left
};

TestLayer::TestLayer(uint32_t width, uint32_t height) : m_CameraController(width, height) {}

void TestLayer::OnAttach() {
  m_mesh.m_primitiveType = Kazel::PrimitiveType::Triangles;
  m_mesh.m_va = Kazel::VertexArray::Create();
  const auto& VBO = Kazel::VertexBuffer::Create(s_Vertices, sizeof(s_Vertices));
  Kazel::BufferLayout layout = {
      {Kazel::ShaderDataType::Float3, "a_Pos"},

  };
  VBO->SetLayout(layout);
  m_mesh.m_va->AddVertexBuffer(VBO);
  m_mesh.m_va->SetIndicesNumber(3);

  m_Shader = Kazel::Shader::Create();
  m_Shader->compileShader(R"(test.vert.glsl)");
  m_Shader->compileShader(R"(test.frag.glsl)");

  Kazel::RenderCommand::CheckError();
}

void TestLayer::OnUpdate(float timeStep) {
  auto& camera = m_CameraController.GetCamera();
  camera.UpdateView();
  Kazel::Renderer::BeginScence(m_CameraController.GetCamera());
  m_CameraController.onUpdate(timeStep);

  Kazel::Renderer::Submit(m_Shader, m_mesh);
  Kazel::Renderer::EndScence();
}

void TestLayer::OnImGuiRender() {
  auto& camera = m_CameraController.GetCamera();
  ImGui::Begin("setting");
  ImGui::Text("camera position:");
  ImGui::SliderFloat3("positions:", glm::value_ptr(m_positon), 0, 10);
  std::string str = std::format("up:{},{},{}", camera.m_up[0], camera.m_up[1], camera.m_up[2]);
  ImGui::Text("%s", str.c_str());
  ImGui::SliderFloat3("target:", glm::value_ptr(m_target), 0, 10);

  camera.m_position = m_positon;
  camera.m_target = m_target;

  ImGui::End();
}

void TestLayer::OnEvent(Kazel::Event& e) {
  m_CameraController.onEvent(e);
}