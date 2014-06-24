//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.


#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <Windows.h>
#include <ctime>


const int fps = 30;

GLuint moon_glow_ratio_uniform;
float moon_glow_ratio_value = 0.3f;
GLuint moon_light_intensity_uniform;
float moon_light_intensity_value=3.7f;
GLuint sky_matrix_uniform;
GLuint backlight_ratio_uniform;
float backlight_ratio_value = 1.25f;
GLuint frontlight_ratio_uniform;
float frontlight_ratio_value = 1.0f;
int window_width;
int window_height;
double parameter_w, parameter_a, parameter_s, parameter_d, parameter_q, parameter_e = 0.0f;

float sky_offset=0.0;

#include "programs.h"
#include "camera.h"
#include "textures.h"
#include "objects.h"

using namespace std;

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
const float camera_base_rel_x = -157.5f;
const float camera_base_target_x = 0;
namespace{
	void MouseMotion(int x, int y){
		float angle_scope = 2.0f;
		float target_offset_scope = 2.0f;
		float sky_scope = 0.05f;
		float window_center_x = window_width/2;
		float mouse_offset = x - window_center_x;
		float ratio = mouse_offset/window_center_x;
		g_sphereCamRelPos.x = camera_base_rel_x +  ratio * angle_scope;
		g_camTarget.z = camera_base_target_x + ratio * target_offset_scope;
		sky_offset = (-1)*sky_scope*ratio;
	}
}

void init(){
	printf("initializing program...\n");
	printf("%f\n", (float)1/(float)fps);
	glm::vec3 temp = glm::vec3(1, 1, 1);
	temp = temp + glm::vec3(1, 1, 1);
	printf("%f\n", temp.x);
	InitializeProgram();
	exit;
	srand(time(NULL));
	try
	{
		loadObjects();
	}
	catch(std::exception &except)
	{
		printf("%s\n", except.what());
		throw;
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);

	glutPassiveMotionFunc (MouseMotion);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
	
	float frame_duration = (float)1000/(float)fps;
	int time_start = glutGet(GLUT_ELAPSED_TIME);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(objects_are_loaded())
	{
		objects_draw();
	}
	glutSwapBuffers();
	int time_end = glutGet(GLUT_ELAPSED_TIME);
	int rendering_time = time_end-time_start;
	if(rendering_time<frame_duration){
		Sleep(frame_duration-rendering_time);
	}
	glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
	window_height=h;
	window_width=w;
	setupShaderData(w, h);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glutPostRedisplay();
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
	if(key==27){
		objects_delete();
		glutLeaveMainLoop();
	}else if(key==32){
		g_bDrawLookatPoint = !g_bDrawLookatPoint;
		printf("Target: %f, %f, %f\n", g_camTarget.x, g_camTarget.y, g_camTarget.z);
		printf("Position: %f, %f, %f\n", g_sphereCamRelPos.x, g_sphereCamRelPos.y, g_sphereCamRelPos.z);
	}else if(key=='z'){
		moon_glow_ratio_value+=0.05f;
		printf("%f", moon_glow_ratio_value);
	}else if(key=='x'){
		moon_glow_ratio_value-=0.05f;
		printf("%f", moon_glow_ratio_value);
	}else if(key=='c'){
		moon_light_intensity_value+=0.05f;
		printf("%f", moon_light_intensity_value);
	}else if(key=='v'){
		moon_light_intensity_value-=0.05f;
		printf("%f", moon_light_intensity_value);
	}else if(key=='1'){
		backlight_ratio_value+=0.05f;
	}else if(key=='2'){
		backlight_ratio_value-=0.05f;
	}/*else if(key=='a'){
		parameter_a+=1;
	}else if(key=='A'){
		parameter_a-=1;
	}else if(key=='s'){
		parameter_s+=1;
	}else if(key=='S'){
		parameter_s-=1;
	}else if(key=='d'){
		parameter_d+=0.1;
	}else if(key=='D'){
		parameter_d-=0.1;
	}*/else{
		dot_control(key);
	}
	//printf("backlight ratio value: %f", backlight_ratio_value);
	printf("\nA: %f\nS: %f\nD:%f\n", parameter_a, parameter_s, parameter_d);
	glutPostRedisplay();
}


unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}
