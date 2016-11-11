
#version 330 core

layout (location = 0) in vec3 inPosition;

out vec3 textureDir;

uniform mat4 viewM;
uniform mat4 projM;

void main()
{
	vec4 pos = projM * viewM * vec4(inPosition, 1.0f);

	gl_Position = pos.xyww;
	
	textureDir = inPosition;
}