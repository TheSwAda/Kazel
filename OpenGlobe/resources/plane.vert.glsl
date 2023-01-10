#version 450 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Color;

out vec3 color;
out vec3 worldPosition;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	gl_PointSize = 8;
	worldPosition = a_Pos;
	color = a_Color;
}