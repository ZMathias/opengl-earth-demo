#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 ColorOffset;
uniform sampler2D color;

void main()
{
    FragColor = texture(color, TexCoord);
} 