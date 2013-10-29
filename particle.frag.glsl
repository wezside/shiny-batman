#version 330

in vec4 v_color;
in float v_lifetime;

out vec4 out_color;

uniform sampler2D u_texture;

void main(void)
{
	vec4 texColor;
	texColor = texture2D(u_texture, gl_PointCoord);
	out_color = v_color * texColor;
	//out_color.a *= v_lifetime;
}