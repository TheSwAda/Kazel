#pragma once

#include "Kazel.h"

class TestLayer : public Kazel::Layer {

 public:
  TestLayer(uint32_t width, uint32_t height);

  void OnAttach() override;

  void OnUpdate(float timeStep) override;

  void OnImGuiRender() override;

  void OnEvent(Kazel::Event& e) override;

 private:
  Kazel::Ref<Kazel::Texture2D> m_texture0;
  Kazel::Ref<Kazel::Texture2D> m_texture1;

  Kazel::Mesh m_mesh;
  Kazel::Ref<Kazel::Shader> m_Shader;
  Kazel::PerspectCameraController m_CameraController;
  static float s_Vertices[];

  //////////////////////////////////////////////////////////////////////////
  glm::vec3 m_positon{3,0,0};
  glm::vec3 m_target{0};
};