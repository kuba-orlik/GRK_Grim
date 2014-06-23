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

GLuint moon_glow_ratio_uniform;
float moon_glow_ratio_value = 0.3f;
GLuint moon_light_intensity_uniform;
float moon_light_intensity_value = 1.5f;

#include "programs.h"
#include "camera.h"
#include "textures.h"
#include "objects.h"

using namespace std;

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();
	
	try
	{
		loadObjects();
	}
	catch(std::exception &except)
	{
		printf("%s\n", except.what());
		throw;
	}

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(objects_are_loaded())
	{
		objects_draw();
	}

	glutSwapBuffers();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
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
	}else{
		camera_control(key);
	}

	glutPostRedisplay();
}


unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}
