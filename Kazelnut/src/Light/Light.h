#pragma once
#include "Kazel.h"

class CLight {
public:
	struct SLightAttritube
	{
		SLightAttritube(glm::vec3 position = glm::vec3{ 1.0f })
			: Position(position), Ambient(1.0f), Diffuse(1.0f), Specular(1.0f)
		{}

		glm::vec3 Position;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};

	CLight();
	void OnAttach();
	void OnUpdate();
	void onImGuiRender();

	SLightAttritube GetLightAttribute() { return m_LightAttribute; }

private:
	SLightAttritube m_LightAttribute;
	Kazel::Ref<Kazel::VertexArray> m_VA;
	Kazel::Ref<Kazel::Shader> m_Shader;
	static float s_Vertices[];
};
