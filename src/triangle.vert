// vertex shader
#version 460 core

layout (location = 0) in vec3 aPos_VS_in;
layout (location = 1) in vec3 aNormal_VS_in;
layout (location = 2) in vec2 aTexCoord_VS_in;

out vec3 WorldPos_TCS_in;
out vec3 Normal_TCS_in;
out vec2 TexCoord_TCS_in;

uniform mat4 model;
uniform mat3 modelMatrix;

void main()
{
	WorldPos_TCS_in = (model * vec4(aPos_VS_in, 1.0)).xyz;
	Normal_TCS_in = modelMatrix * aNormal_VS_in;
	TexCoord_TCS_in = aTexCoord_VS_in;
}