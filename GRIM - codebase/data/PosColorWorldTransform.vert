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

uniform vec3 the_dot_position;
uniform float dot_light_intensity;

uniform vec3 observer_location;

uniform int phong_parameter;


float phongFunc(float cos, int parameter){
	return pow(cos, parameter);
}

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

	vec4 vertex_in_world_space = modelToWorldMatrix * position;
	vec4 normal_in_world_space = modelToWorldMatrix * normal;
	vec3 vertex_in_world_space3 = vec3(vertex_in_world_space.x, vertex_in_world_space.y, vertex_in_world_space.z);
	vec3 normal_in_world_space3 = normalize(vec3(normal_in_world_space.x, normal_in_world_space.y, normal_in_world_space.z));
	//vec3 vector_from_dot_to_vertex = normalize(vec3(vertex_in_world_space.x, vertex_in_world_space.y, vertex_in_world_space.z) - the_dot_position);
	vec3 vector_from_dot_to_vertex = the_dot_position - vec3(vertex_in_world_space.x, vertex_in_world_space.y, vertex_in_world_space.z);
	float distance_from_dot = length(vector_from_dot_to_vertex);
	vector_from_dot_to_vertex = normalize(vector_from_dot_to_vertex);
	float cos_of_angle_between_normal_and_dot_vector = dot(normal_in_world_space3, vector_from_dot_to_vertex);

	float lambert_intensity = 1/(2*3.14)*1/distance_from_dot*cos_of_angle_between_normal_and_dot_vector * dot_light_intensity;
	vec4 dot_color = vec4(1, 1, 1, 1);
	vec4 lambert_light = dot_color * lambert_intensity;

	vec3 observer_vector = observer_location - vec3(vertex_in_world_space.x, vertex_in_world_space.y, vertex_in_world_space.z);
	//vec3 observer_vector = (-1) * vec3(vertex_in_world_space.x, vertex_in_world_space.y, vertex_in_world_space.z);
	//observer_vector = -1 * observer_vector;

	//vec3 reflection_vector = -2 * dot(normal_in_world_space3, vector_from_dot_to_vertex) * normal_in_world_space3 + vector_from_dot_to_vertex;
	vec3 reflection_vector = reflect(vector_from_dot_to_vertex, normal_in_world_space3);
	reflection_vector = normalize(reflection_vector);
	observer_vector = normalize(observer_vector);
	float refl_obsrv_angl_cos = dot(reflection_vector, observer_vector);
	float phong_ratio;
	float dist_to_vert = length(vertex_in_world_space3 - observer_location);
	float dist_to_light = length(the_dot_position - observer_location);
	if(dist_to_vert<dist_to_light){
		phong_ratio = phongFunc(refl_obsrv_angl_cos, phong_parameter);
	}else{
		phong_ratio = 0;
	}
	phong_ratio = clamp(phong_ratio, 0, 1);		
	vec4 phong_color = vec4(1, 1,1, 1);
	vec4 phong_light = phong_color * phong_ratio;

	vec4 ambient_light = vec4(0, 0, 0, 1);

	interpColor = moon_light_color * color * cosAngIncidence * moon_light_intensity + lambert_light + phong_light + ambient_light;
	//interpColor = moon_light_color * color * cosAngIncidence * moon_light_intensity + phong_light;
	//interpColor = phong_light;
	uv=vertex_uv;
	//interpColor = normal;
}
