
#version 330 core

out vec4 color;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 lightPos;
};

in vec2 texC;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;
uniform vec3 viewPos;

uniform Material mat;
uniform Light light;

void main()
{
	//vec3 ambient = light.ambient * mat.ambient;
	vec3 ambient = light.ambient * vec3(texture(diffuseMap, texC));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = diff * light.diffuse * mat.diffuse;
	vec3 diffuse = diff * light.diffuse * vec3(texture(diffuseMap, texC));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f),mat.shininess);
	//vec3 specular = spec * light.specular * mat.specular;
	vec3 specular = spec * light.specular * vec3(texture(specularMap, texC));
	
	//vec4 objectColor = texture(diffuseMap, texC);
	//vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3 emission = vec3(texture(emissionMap, texC).rrr * 0.1f);

	//vec4 result = objectColor * vec4(ambient + diffuse + specular, 1.0f);
	vec4 result = vec4(ambient + diffuse + specular +emission ,1.0f);

	color = result;
}
