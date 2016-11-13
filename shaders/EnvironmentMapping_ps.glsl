#version 330 core

in vec3 normal;
in vec3 fragPos;

uniform samplerCube envMap;
uniform vec3 cameraPos;

out vec4 color;

void main()
{
	float ratio = 1.00f / 1.52f;

	vec3 I = normalize(fragPos - cameraPos);
	vec3 R = refract(I, normalize(normal), ratio);
	
	color = texture(envMap, R);
}