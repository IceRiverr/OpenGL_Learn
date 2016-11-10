
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec2 texC;
out vec3 normal;
out vec3 fragPos;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projM;

void main()
{
	gl_Position = projM * viewM * modelM * vec4(inPosition, 1.0f);
	texC = vec2(inTexCoord.x, inTexCoord.y);
	normal = inNormal;
	normal = mat3(transpose(inverse(modelM))) * inNormal;
	fragPos = vec3(modelM * vec4(inPosition, 1.0f));
}