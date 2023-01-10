#include "GlobeLayer.h"
#include <imgui/imgui.h>
#include <OpenGlobe/GeographicGridEllipsoidTessellator.h>
#include <glad/glad.h>

GlobeLayer::GlobeLayer() : m_cameraController(nullptr), m_ellipsoid{Ellipsoid::Wgs84} {}

void GlobeLayer::OnAttach() {
  uint32_t width = Kazel::Application::Get().GetWindow().GetWidth();
  uint32_t height = Kazel::Application::Get().GetWindow().GetHeight();

  m_cameraController = CreateScope<CameraLookAtPoint>(width, height, m_ellipsoid);

  m_globeMesh = Kazel::GeographicGridEllipsoidTessellator::Compute(m_ellipsoid, 64, 32, Kazel::GGEVA::Position);
  m_diffuse = Kazel::Texture2D::Create("world_topo_bathy_200411_3x5400x2700.jpg");

  m_globeShader = Kazel::Shader::Create();
  m_globeShader->compileShader(R"(globe.frag.glsl)");
  m_globeShader->compileShader(R"(globe.vert.glsl)");
  m_globeShader->use();
  m_globeShader->setUniform("u_globeOneOverRadiiSquared", glm::vec3(m_ellipsoid.m_oneOverRadiiSquared));
  m_globeShader->setUniform("u_gridLineWidth", glm::vec2{1.0, 1.0});
  m_globeShader->setUniform("u_gridResolution", glm::vec2{0.05, 0.05});
}

void GlobeLayer::OnUpdate(float timeStep) {
  Kazel::Renderer::BeginScence(m_cameraController->GetCamera());
  m_globeShader->use();
  m_diffuse->Bind(0);
  Kazel::Renderer::Submit(m_globeShader, m_globeMesh, m_tranform);
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
