#version 460 core

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 ColorOffset;

void main()
{
    FragColor = vec4(Pos + ColorOffset, 1.0f);
} 