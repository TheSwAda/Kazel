#version 450 core

layout (location = 0) in vec3 a_Pos;

out vec3 worldPosition;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	worldPosition = a_Pos;
}