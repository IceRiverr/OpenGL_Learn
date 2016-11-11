
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;

out vec2 texC;

void main()
{
	gl_Position = vec4(inPosition, 1.0f);
	texC = inTexcoord;
}