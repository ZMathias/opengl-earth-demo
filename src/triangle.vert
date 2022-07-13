#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoord;

uniform sampler2D tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelMatrix;

void main()
{
	vec4 height = texture(tex, aPos.xy);
    gl_Position = projection * view * model * vec4(aPos*height.x / 2, 1.0);
	Pos = height.xyz;
}