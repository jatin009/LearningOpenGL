#version 330 core

in vec2 fragTexCoord;

out vec4 color;
uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

uniform float u_MixVal;

void main()
{
	color = mix( texture(u_Texture1, fragTexCoord), texture(u_Texture2, vec2(1.0f - fragTexCoord.x, fragTexCoord.y)), u_MixVal) * u_Color;
};