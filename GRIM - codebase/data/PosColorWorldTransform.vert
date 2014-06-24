#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 vertex_uv;

smooth out vec4 interpColor;
out vec2 uv;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

uniform float glow_ratio;
uniform float moon_light_intensity;
uniform float backlight_ratio;
uniform float frontlight_ratio;

void main()
{
	vec4 temp = modelToWorldMatrix * position;
	temp = worldToCameraMatrix * temp;
	gl_Position = cameraToClipMatrix * temp;


	vec4 light_direction = vec4(1.0, 0.0, 1.0, 1.0);
	vec4 moon_light_color = vec4(0.7, 0.7, 1.0, 1.0);

	mat4 finalMatrix4 = worldToCameraMatrix * modelToWorldMatrix;
	mat3 finalMatrix3;
	finalMatrix3[0] = vec3(finalMatrix4[0].x, finalMatrix4[0].y, finalMatrix4[0].z);
	finalMatrix3[1] = vec3(finalMatrix4[1].x, finalMatrix4[1].y, finalMatrix4[1].z);
	finalMatrix3[2] = vec3(finalMatrix4[2].x, finalMatrix4[2].y, finalMatrix4[2].z);

	vec3 normCamSpace = normalize(finalMatrix3 * vec3(normal.x, normal.y, normal.z));
	light_direction = worldToCameraMatrix * light_direction;
	vec3 lightDirCamSpace = normalize(vec3(light_direction.x, light_direction.y, light_direction.z));
	float cosAngIncidence = dot(normCamSpace, lightDirCamSpace);
	cosAngIncidence  = cosAngIncidence * (1-glow_ratio)+glow_ratio;
	if(cosAngIncidence<0){
		cosAngIncidence = -1 * backlight_ratio * cosAngIncidence;
	}else{
		cosAngIncidence = cosAngIncidence * frontlight_ratio;
	}
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	interpColor = moon_light_color * color * cosAngIncidence * moon_light_intensity;
	uv=vertex_uv;
	//interpColor = normal;
}
