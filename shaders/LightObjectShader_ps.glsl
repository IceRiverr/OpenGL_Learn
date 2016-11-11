
#version 330 core

struct Material
{
	sampler2D diffuseMap0;
	sampler2D specularMap0;
	float shininess;
};

struct DirLight
{
	vec3 lightDir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 lightPos;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
};

struct SpotLight
{
	vec3 lightPos;
	vec3 lightDir;
	float cutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

in vec2 texC;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Material mat;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
	
	color = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.lightDir);

	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f),mat.shininess);

	vec3 ambient =	light.ambient	* vec3(texture(mat.diffuseMap0, texC));
	vec3 diffuse =	light.diffuse	* diff * vec3(texture(mat.diffuseMap0, texC));
	vec3 specular = light.specular	* spec * vec3(texture(mat.specularMap0, texC));
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPos - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f),mat.shininess);

	float dist = length(light.lightPos - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	vec3 ambient =	light.ambient *	 vec3(texture(mat.diffuseMap0, texC));
	vec3 diffuse =	light.diffuse *  diff * vec3(texture(mat.diffuseMap0, texC));
	vec3 specular =	light.specular * spec *  vec3(texture(mat.specularMap0, texC));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f),mat.shininess);
	
	float dist = length(light.lightPos - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	float theta = dot(lightDir, normalize(-light.lightDir));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp(theta - light.outerCutoff, 0.0f, 1.0f) / epsilon;

	vec3 ambient =	light.ambient  * vec3(texture(mat.diffuseMap0, texC));
	vec3 diffuse =	light.diffuse  * diff * vec3(texture(mat.diffuseMap0, texC));
	vec3 specular = light.specular * spec * vec3(texture(mat.specularMap0, texC));
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}
