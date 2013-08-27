#version 330

// attribute: initial setup
layout(location=0) in vec4  in_sPosition;
layout(location=1) in vec4  in_ePosition;
layout(location=2) in vec4  in_color;
layout(location=3) in float in_lifetime;

// varying: what we will pass to the fragment shader
out vec4 v_color;
out float v_lifetime;

// client-app vars 
uniform float u_time;
uniform vec3 u_centerPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{	
	if (u_time <= in_lifetime)
	{
		gl_Position.xyz = in_sPosition.xyz + u_time * in_ePosition.xyz;
		//gl_Position.xyz += u_centerPosition.xyz;
		gl_Position.w = 1.0;
	}
	else
		gl_Position = vec4(-1000, -1000, 0, 0);

	v_lifetime = 1.0 - (u_time / in_lifetime);
	v_lifetime = clamp(v_lifetime, 0.0, 1.0);
	//gl_PointSize = (v_lifetime * v_lifetime) * 10.0;
	v_color = in_color;	
}