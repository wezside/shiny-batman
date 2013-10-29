#version 330

// attribute: initial setup
layout(location=0) in vec4  in_sPosition;
layout(location=1) in vec4  in_ePosition;
layout(location=2) in vec4  in_color;
layout(location=3) in float in_lifetime;
layout(location=4) in float in_velocity;

// varying: what we will pass to the fragment shader
out vec4 v_color;
out float v_lifetime;

// client-app vars 
uniform float u_time;
uniform vec3 u_centerPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

void main(void)
{	
	vec4 v = in_sPosition + u_time * in_velocity * (in_ePosition - in_sPosition);
	if (u_time <= in_lifetime)
	{
		vec4 v = in_sPosition + u_time * in_velocity * (in_ePosition - in_sPosition);
 		v.y += sin(waveWidth * (u_time * in_velocity) + waveTime) * cos(waveWidth * v.z + waveTime) * waveHeight;
		gl_Position = v;
	}
	else
		gl_Position = in_sPosition;

	v_lifetime = 1.0 - (u_time / in_lifetime);
	v_lifetime = clamp(v_lifetime, 0.0, 1.0);
	// gl_PointSize = (v_lifetime * v_lifetime) * 3.0 * in_velocity;
	gl_PointSize = in_lifetime * 3.0 * in_velocity;
	v_color = in_color;
}