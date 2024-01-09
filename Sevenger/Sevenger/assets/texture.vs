#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texCoord;

out vec3 f_color;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	f_color = a_color;
	texCoord = vec2(a_texCoord.x, a_texCoord.y);
}