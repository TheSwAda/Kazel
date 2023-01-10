#pragma once

#include <Kazel.h>
#include "OpenGlobe/GeographicGridEllipsoidTessellator.h"

using namespace Kazel;
class GlobeLayer : public Kazel::Layer {
 public:
  GlobeLayer();

  void OnAttach() override;

  void OnUpdate(float timeStep) override;

  void OnImGuiRender() override;

  void OnEvent(Kazel::Event& e) override;

 private:
  int m_subdivisionNum{3};
  glm::mat4 m_tranform{1.0f};

  double m_pressXPositon{0};
  double m_pressYPositon{0};

  bool m_pressed{false};

  Ellipsoid m_ellipsoid;

  Mesh m_globeMesh;
  Ref<Texture2D> m_diffuse;
  Ref<Shader> m_globeShader;
  Scope<CameraLookAtPoint> m_cameraController;
  //Kazel::PerspectCameraController m_cameraController;
};