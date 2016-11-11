
#version 330 core

out vec4 color;

in vec2 texC;

uniform sampler2D mainMap;

void main()
{
	color = texture(mainMap, vec2(texC.x, 1.0f - texC.y));
}
