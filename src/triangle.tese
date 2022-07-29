// evaluation shader
#version 460 core

layout (triangles, equal_spacing, ccw) in;

uniform mat4 view;
uniform mat4 projection;
uniform mat3 modelMatrix;

uniform sampler2D heightMap_ES_in0;
uniform sampler2D heightMap_ES_in1;

in vec3 WorldPos_ES_in[];
in vec3 Normal_ES_in[];
in vec2 TexCoord_ES_in[];
in vec3 cameraPos_ES_in[];
in float heightMultiplier_ES_in[];

out vec3 WorldPos_FS_in;
out vec3 Normal_FS_in;
out vec2 TexCoord_FS_in;
out vec3 cameraPos_FS_in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec3 GetDisplacement(vec2 texCoord_)
{
	vec3 result = vec3(0.0f);
	if (texCoord_.x < 0.5)
	{
		vec2 texCoord = vec2(texCoord_.x * 2, texCoord_.y);
		result = texture(heightMap_ES_in0, texCoord).rgb;
	}
	else
	{
		vec2 texCoord = vec2((texCoord_.x - 0.5f) * 2, texCoord_.y);
		result = texture(heightMap_ES_in1, texCoord).rgb;
	}
	return result;
}

void main()
{
    WorldPos_FS_in = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);
	Normal_FS_in = normalize(interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]));
	TexCoord_FS_in = interpolate2D(TexCoord_ES_in[0], TexCoord_ES_in[1], TexCoord_ES_in[2]);
	cameraPos_FS_in = cameraPos_ES_in[0];

	vec3 displacement = GetDisplacement(TexCoord_FS_in);
	WorldPos_FS_in += displacement * Normal_FS_in * heightMultiplier_ES_in[0];
	gl_Position = projection * view * vec4(WorldPos_FS_in, 1.0f);
}