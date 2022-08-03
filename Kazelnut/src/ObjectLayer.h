#pragma once

#include "Kazel.h"
#include "Light/Light.h"

class ObjectLayer : public Kazel::Layer {
public:
	struct SMaterial
	{
		Kazel::Ref<Kazel::Texture2D> m_Diffuse;
		Kazel::Ref<Kazel::Texture2D> m_Specular;
		int Shininess;
	};

	ObjectLayer(uint32_t width, uint32_t height);

	void OnAttach() override;

	void OnUpdate(float timeStep) override;

	void OnImGuiRender() override;

	void OnEvent(Kazel::Event& e) override;

private:
	CLight m_Light;

	Kazel::Ref<Kazel::VertexArray> m_VA;
	Kazel::Ref<Kazel::Shader> m_Shader;
	SMaterial m_Material;
	float m_Angle{ glm::radians(40.0f) };
	Kazel::PerspectCameraController m_CameraController;
	static float s_Vertices[];
};