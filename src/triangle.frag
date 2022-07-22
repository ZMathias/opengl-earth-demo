#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentWorldPos;
out vec4 FragColor;

uniform vec3 ColorOffset;
uniform vec3 cameraPos;

struct Material
{
	sampler2D diffuse;
	//sampler2D specular;
	float shininess;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform Material material;
uniform PointLight pointLight;

vec3 CalcPointLightImpact(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    // attenuation
    float distance    = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec;
	
    //ambient  *= attenuation;
    //diffuse  *= attenuation;
    //specular *= attenuation;
	
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 ambient = vec3(0.2f);

    vec3 aNormal = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragmentWorldPos);
	vec3 result = vec3(0.0f);
	result += CalcPointLightImpact(pointLight, aNormal, FragmentWorldPos, viewDir);
    FragColor = vec4(result * ambient, 1.0f);
} 