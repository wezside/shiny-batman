#version 330

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_TexCoord;

out vec4 ex_Color;
out vec2 v_uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * in_Position;
	ex_Color = in_Color;
	v_uv = in_TexCoord;
}