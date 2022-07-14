#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform sampler2D tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelMatrix;

uniform float heightMultiplier;

vec2 spherical(vec3 a)
{
	float longitude = atan(a.y, a.x);
	float latitude=acos(a.z);
	return vec2(longitude, latitude);
}

void main()
{
	vec4 height = texture(tex, aTexCoord);
    gl_Position = projection * view * model * vec4(aPos + aNormal*height.xyz * heightMultiplier, 1.0);
	TexCoord = aTexCoord;
}