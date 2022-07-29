// control shader
#version 460 core

layout (vertices = 3) out;

uniform sampler2D heightMap_TCS_in0;
uniform sampler2D heightMap_TCS_in1;
uniform vec3 cameraPos;
uniform float heightMultiplier;

in vec3 WorldPos_TCS_in[];
in vec3 Normal_TCS_in[];
in vec2 TexCoord_TCS_in[];

out vec3 WorldPos_ES_in[];
out vec3 Normal_ES_in[];
out vec2 TexCoord_ES_in[];
out vec3 cameraPos_ES_in[];
out float heightMultiplier_ES_in[];

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;
	return uint(clamp(250/(AvgDistance - 25), 1, 150));
}

vec3 GetDisplacement(vec3 normal, vec2 texCoord_)
{
	vec3 result = vec3(0.0f);
	if (texCoord_.x < 0.5)
	{
		vec2 texCoord = vec2(texCoord_.x * 2, texCoord_.y);
		result = texture(heightMap_TCS_in0, texCoord).rgb;
	}
	else
	{
		vec2 texCoord = vec2((texCoord_.x - 0.5f) * 2, texCoord_.y);
		result = texture(heightMap_TCS_in1, texCoord).rgb;
	}
	return result * normal * heightMultiplier;
}

void main()
{
    WorldPos_ES_in[gl_InvocationID] = WorldPos_TCS_in[gl_InvocationID];
    Normal_ES_in[gl_InvocationID] = Normal_TCS_in[gl_InvocationID];
	TexCoord_ES_in[gl_InvocationID] = TexCoord_TCS_in[gl_InvocationID];
	
	cameraPos_ES_in[0] = cameraPos;
	heightMultiplier_ES_in[0] = heightMultiplier;

	vec3 displacement0 = GetDisplacement(Normal_TCS_in[0], TexCoord_TCS_in[0]);
	vec3 displacement1 = GetDisplacement(Normal_TCS_in[1], TexCoord_TCS_in[1]);
	vec3 displacement2 = GetDisplacement(Normal_TCS_in[2], TexCoord_TCS_in[2]);
	
	float CamToVert0 = distance(cameraPos, WorldPos_TCS_in[0] + displacement0);
	float CamToVert1 = distance(cameraPos, WorldPos_TCS_in[1] + displacement1);
	float CamToVert2 = distance(cameraPos, WorldPos_TCS_in[2] + displacement2);
	
	gl_TessLevelOuter[0] = GetTessLevel(CamToVert1, CamToVert2);
	gl_TessLevelOuter[1] = GetTessLevel(CamToVert2, CamToVert0);
	gl_TessLevelOuter[2] = GetTessLevel(CamToVert0, CamToVert1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}