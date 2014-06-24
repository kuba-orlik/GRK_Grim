#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 vertex_uv;

smooth out vec4 interpColor;
out vec2 uv;

uniform mat4 sky_matrix;
void main()
{
	gl_Position = sky_matrix * position;
	//gl_Position = position;
	uv=vertex_uv;
	//interpColor = normal;
}
