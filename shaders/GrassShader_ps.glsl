
#version 330 core

out vec4 color;

in vec2 texC;

uniform sampler2D grassMap;

void main()
{
	vec2 newTexC = vec2(texC.x, 1.0f - texC.y);

	vec4 texColor = texture(grassMap, newTexC);
	if(texColor.a < 0.1f)
		discard;
	color = texColor;
}
