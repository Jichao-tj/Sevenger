#version 330 core
out vec4 fragment_color;

in vec3 f_color;
in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
	fragment_color = texture(texture1, texCoord);
}