// fragment shader
#version 460 core

in vec3 WorldPos_FS_in;
in vec3 Normal_FS_in;
in vec2 TexCoord_FS_in;
in vec3 cameraPos_FS_in;
out vec4 FragColor;

uniform vec3 ColorOffset;

struct Material
{
	sampler2D diffuseSlice0;
	sampler2D diffuseSlice1;
    sampler2D debug;
	float shininess;
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;

vec3 GetDiffuse(vec2 texCoord_)
{
    vec3 result = vec3(0.0f);
	if (texCoord_.x < 0.5)
    {
	    vec2 texCoord = vec2(texCoord_.x * 2, texCoord_.y);
	    result = texture(material.diffuseSlice0, texCoord).rgb;
	}
	else
    {
        vec2 texCoord = vec2((texCoord_.x - 0.5f) * 2, texCoord_.y);
        result = texture(material.diffuseSlice1, texCoord).rgb;
	}
    return result;
}

vec3 CalcDirLightImpact(DirLight light, vec3 normal, vec3 viewDir)
{
    // we only care about the light direction
    vec3 lightDir = -light.direction;
	
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 diffTexSample = GetDiffuse(TexCoord_FS_in);

    // combine results
    vec3 ambient  = light.ambient  * diffTexSample;
    vec3 diffuse  = light.diffuse  * diff * diffTexSample;
    vec3 specular = light.specular * spec;
	
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 ambient = vec3(0.2f);

    vec3 aNormal = normalize(Normal_FS_in);
	vec3 viewDir = normalize(cameraPos_FS_in - WorldPos_FS_in);
	vec3 result = vec3(0.0f);
	
	result += CalcDirLightImpact(dirLight, aNormal, viewDir);
    FragColor = vec4(result * ambient, 1.0f);
} 