#include "GlobeLayer.h"
#include <imgui/imgui.h>
#include <OpenGlobe/GeographicGridEllipsoidTessellator.h>
#include <glad/glad.h>

GlobeLayer::GlobeLayer() : m_cameraController(nullptr), m_ellipsoid{Ellipsoid::Wgs84} {}
unsigned int VAO;
void GlobeLayer::OnAttach() {
  uint32_t width = Kazel::Application::Get().GetWindow().GetWidth();
  uint32_t height = Kazel::Application::Get().GetWindow().GetHeight();

  m_cameraController = CreateScope<CameraLookAtPoint>(width, height, m_ellipsoid);
  //m_cameraController->ViewPort(m_ellipsoid,Geodetic3D{0,0,0});
  m_globeMesh = Kazel::GeographicGridEllipsoidTessellator::Compute(m_ellipsoid, 64, 32, Kazel::GGEVA::Position);
  m_diffuse = Kazel::Texture2D::Create("world_topo_bathy_200411_3x5400x2700.jpg");

  m_globeShader = Kazel::Shader::Create();
  m_globeShader->compileShader(R"(globe.frag.glsl)");
  m_globeShader->compileShader(R"(globe.vert.glsl)");
  m_globeShader->use();
  m_globeShader->setUniform("u_globeOneOverRadiiSquared", glm::vec3(m_ellipsoid.m_oneOverRadiiSquared));
  m_globeShader->setUniform("u_gridLineWidth", glm::vec2{1.0, 1.0});
  m_globeShader->setUniform("u_gridResolution", glm::vec2{0.05, 0.05});

  //////////////////////////////////////////////////////////////////////////
  //test
  m_demoShader = Kazel::Shader::Create();
  m_demoShader->compileShader(R"(plane.frag.glsl)");
  m_demoShader->compileShader(R"(plane.vert.glsl)");
  float vertices[] = {0 + 0.5, 0,    Ellipsoid::Wgs84.z, 20000, 20000, Ellipsoid::Wgs84.z,
                      0.5,     20000, Ellipsoid::Wgs84.z, 0,    0,    Ellipsoid::Wgs84.z};

  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void GlobeLayer::OnUpdate(float timeStep) {
  Kazel::Renderer::BeginScence(m_cameraController->GetCamera());
  m_diffuse->Bind(0);
  Kazel::Renderer::Submit(m_globeShader, m_globeMesh, m_tranform);

  m_demoShader->use();
  glBindVertexArray(VAO);
  auto vp = m_cameraController->GetCamera().ViewPorjection();
  m_demoShader->setUniform("u_ViewProjection", vp);
  m_demoShader->setUniform("u_Transform", m_tranform);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawArrays(GL_POINTS, 3, 1);
}

void GlobeLayer::OnImGuiRender() {
  ImGui::Begin("setting");
  //bool changed = ImGui::InputInt("subdvision number:", &m_subdivisionNum, 1);
  ImGui::Text("%lf", m_cameraController->GetCamera().Distance());
  ImGui::End();
}

void GlobeLayer::OnEvent(Kazel::Event& e) {
  m_cameraController->OnEvent(e);
}
