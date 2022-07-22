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

const float TWOOVERPI = 0.63661977236f;
const float PI = 3.14159265358979323846f;
const float TWOPI = 6.28318530717958647692f;
const float ONEOVERPI = 0.31830988618f;
const float ONEOVERTWOPI = 0.15915494309;
const float radius = 10.0f;

void main()
{
	vec2 uv;
	uv.x = 0.0f;
	uv.y = 0.0f;
	if (aPos.z > 0)
	{
		uv.x = asin(aPos.x) * ONEOVERTWOPI + 0.25f;
		uv.y = asin(aPos.y) * ONEOVERPI + 0.5f;
	}
	else
	{
		uv.x = asin(aPos.x) * ONEOVERTWOPI - 0.75;
		uv.y = asin(aPos.x) * ONEOVERTWOPI - 0.75;
	}
	vec3 height = texture(tex, uv).xyz;
    gl_Position = projection * view * model * vec4(aPos + aNormal * height / 5, 1.0);
	if (uv.x < 0.0f)
	{
		Pos = vec3(0.0f);
	}
	
	if (uv.y < 0.0f)
	{
		Pos = vec3(10.0f);
	}

	Pos = height.xyz;
}