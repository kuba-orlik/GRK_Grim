#version 330

smooth in vec4 interpColor;
uniform vec4 baseColor;

out vec4 outputColor;
in vec2 uv;
uniform sampler2D myTextureSampler;

void main()
{
	//vec2 temp = vec2(0.0, 0.0);
	vec4 texture_color = texture(myTextureSampler, uv);
	outputColor = texture_color.rgba * interpColor;
	//outputColor = vec4(outputColor.x, outputColor.y, outputColor.z, 0);
	//outputColor = interpColor;
	if(texture_color.x==0 && texture_color.y==0 && texture_color.z==0){
		discard;
	}
}
