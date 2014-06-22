#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 normal;

smooth out vec4 interpColor;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
	vec4 temp = modelToWorldMatrix * position;
	temp = worldToCameraMatrix * temp;
	gl_Position = cameraToClipMatrix * temp;


	vec4 light_direction = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 light_intensity = vec4(0.2, 0.2, 0.2, 0.2);

	mat4 finalMatrix4 = worldToCameraMatrix * modelToWorldMatrix;
	mat3 finalMatrix3;
	finalMatrix3[0] = vec3(finalMatrix4[0].x, finalMatrix4[0].y, finalMatrix4[0].z);
	finalMatrix3[1] = vec3(finalMatrix4[1].x, finalMatrix4[1].y, finalMatrix4[1].z);
	finalMatrix3[2] = vec3(finalMatrix4[2].x, finalMatrix4[2].y, finalMatrix4[2].z);

	vec3 normCamSpace = normalize(finalMatrix3 * vec3(normal.x, normal.y, normal.z));
	vec3 lightDirCamSpace = normalize(finalMatrix3 * vec3(light_direction.x, light_direction.y, light_direction.z));
	float cosAngIncidence = dot(normCamSpace, lightDirCamSpace);
	//cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	interpColor = color* cosAngIncidence;
}
