#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "lodepng.h"
#include "ShaderProgram.h"
#include "myCube.h"
#include "myTeapot.h"

float speed_x = 0;
float speed_y = 0;
float aspectRatio = 1;
ShaderProgram *sp;

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_LEFT) 
			speed_x = -PI / 2;
        if(key == GLFW_KEY_RIGHT)
			speed_x = PI / 2;
        if(key == GLFW_KEY_UP)
			speed_y = PI / 2;
        if(key == GLFW_KEY_DOWN)
			speed_y = -PI / 2;
    }
    if(action == GLFW_RELEASE){
        if(key == GLFW_KEY_LEFT)
			speed_x = 0;
        if(key == GLFW_KEY_RIGHT)
			speed_x = 0;
        if(key == GLFW_KEY_UP)
			speed_y = 0;
        if(key == GLFW_KEY_DOWN)
			speed_y = 0;
    }
}

void windowResizeCallback(GLFWwindow* window, int width, int height) 
{
    if(height == 0)
		return;
    aspectRatio = (float)width / (float)height;
    glViewport(0,0,width,height);
}

void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	sp = new ShaderProgram("shader/v_simplest.glsl",NULL,"shader/f_simplest.glsl");
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete sp;
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 V=glm::lookAt(
        glm::vec3(0.0f,0.0f,-5.0f),
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, 1.0f, 1.0f, 50.0f);
    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Compute model matrix
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Compute model matrix
	glm::vec4 lp=glm::vec4(0.0f, 0.0f, -6.0f, -1.0f);
    sp->use();

    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	glUniformMatrix4fv(sp->u("lp"),1,false,glm::value_ptr(lp));

    glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("color"));
	glEnableVertexAttribArray(sp->a("normal"));

    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,myTeapotVertices);
	glVertexAttribPointer(sp->a("color"),4,GL_FLOAT,false,0,myTeapotColors);
	glVertexAttribPointer(sp->a("normal"),4,GL_FLOAT,false,0,myTeapotVertexNormals);

    glDrawArrays(GL_TRIANGLES,0,myTeapotVertexCount);

    glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));

    glfwSwapBuffers(window);
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if(!glfwInit()){
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);
	if(!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	GLenum err;
	if((err=glewInit()) != GLEW_OK){ 
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	initOpenGLProgram(window);
	float angle_x=0;
	float angle_y=0;
	glfwSetTime(0);
	
	while(!glfwWindowShouldClose(window)){
        angle_x += speed_x * glfwGetTime();
		angle_y += speed_y * glfwGetTime();
        glfwSetTime(0);
		drawScene(window, angle_x, angle_y);
		glfwPollEvents();
	}
	freeOpenGLProgram(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
