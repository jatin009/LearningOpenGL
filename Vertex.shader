#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform float u_xShift;
out vec2 fragTexCoord;

void main()
{
	gl_Position = vec4(position.x + u_xShift, position.y, 0.0f, 1.0f);
	fragTexCoord = texCoord;
};