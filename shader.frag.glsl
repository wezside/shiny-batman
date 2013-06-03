#version 330

in vec2 v_uv;
in vec4 ex_Color;
out vec4 out_Color;

uniform sampler2D s_texture;

void main(void)
{
	//out_Color = ex_Color;
	out_Color = texture2D(s_texture, v_uv).rgba;
}