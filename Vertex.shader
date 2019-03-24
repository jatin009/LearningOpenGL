#version 330 core
layout(location = 0) in vec3 position;

uniform float u_xShift;

void main()
{
	gl_Position = vec4(position.x + u_xShift, position.y, position.z, 1.0f);
};