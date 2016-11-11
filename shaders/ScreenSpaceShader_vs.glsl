
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;

out vec2 texC;

uniform mat4 modelM;

void main()
{
	gl_Position = modelM * vec4(inPosition, 1.0f);
	texC = inTexcoord;
}
