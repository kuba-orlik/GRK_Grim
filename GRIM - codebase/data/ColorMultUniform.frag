#version 330

smooth in vec4 interpColor;
uniform vec4 baseColor;

out vec4 outputColor;
in vec2 uv;
uniform sampler2D myTextureSampler;

void main()
{
	//vec2 temp = vec2(0.0, 0.0);
	outputColor = texture( myTextureSampler, uv ).rgba * interpColor;
	//outputColor = interpColor;
}
