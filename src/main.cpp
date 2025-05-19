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
#include "Board.h"

float speed_x = 0;
float speed_y = 0;
float aspectRatio = 1;
ShaderProgram *sp;
GLuint whiteTileTexture;
GLuint blackTileTexture;

void genereteBoard(void){
	int vertexIndex = 0;
	bool white = true;
	for(int i = 0; i < BOARD_SIZE; i++){
		white = !white;
		for (int j = 0; j < BOARD_SIZE; j++){
			float offsetX = (j - BOARD_SIZE / 2);
			float offsetZ = (i - BOARD_SIZE / 2);
			white = !white;
			for(int k = 0; k < TILE_VERTEX_COUNT; k++){
				int base = k * 4;
				float px = tileVertices[base + 0] + offsetX;
				float py = tileVertices[base + 1];
				float pz = tileVertices[base + 2] + offsetZ;
				float pw = tileVertices[base + 3];
				boardVertices[vertexIndex * 4 + 0] = px;
				boardVertices[vertexIndex * 4 + 1] = py;
				boardVertices[vertexIndex * 4 + 2] = pz;
				boardVertices[vertexIndex * 4 + 3] = pw;
				boardNormals[vertexIndex * 4 + 0] = 0.0f;
				boardNormals[vertexIndex * 4 + 1] = 1.0f;
				boardNormals[vertexIndex * 4 + 2] = 0.0f;
				boardNormals[vertexIndex * 4 + 3] = 0.0f;
				boardTexCoords[vertexIndex * 2 + 0] = tileTexCoords[k * 2 + 0];
                boardTexCoords[vertexIndex * 2 + 1] = tileTexCoords[k * 2 + 1];
				if(white){
					boardColors[vertexIndex * 4 + 0] = WHITE_TILE_COLOR_R;
					boardColors[vertexIndex * 4 + 1] = WHITE_TILE_COLOR_G;
					boardColors[vertexIndex * 4 + 2] = WHITE_TILE_COLOR_B;
					boardColors[vertexIndex * 4 + 3] = 1.f;
					boardTextures[vertexIndex] = 1.f; 
				}else{
					boardColors[vertexIndex * 4 + 0] = BLACK_TILE_COLOR_R;
					boardColors[vertexIndex * 4 + 1] = BLACK_TILE_COLOR_G;
					boardColors[vertexIndex * 4 + 2] = BLACK_TILE_COLOR_B;
					boardColors[vertexIndex * 4 + 3] = 1.f;
					boardTextures[vertexIndex] = 0.f; 
				}
				vertexIndex++;
			}
		}
	}
}

GLuint readTexture(const char* filename){
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

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
	sp = new ShaderProgram("shader/v_textured.glsl",NULL,"shader/f_textured.glsl");
	whiteTileTexture = readTexture("texture/white-tile.png");
	blackTileTexture = readTexture("texture/black-tile.png");
	genereteBoard();
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete sp;
	glDeleteTextures(1, &whiteTileTexture);
	glDeleteTextures(1, &blackTileTexture);
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 V=glm::lookAt(
        glm::vec3(0.0f,5.0f,-12.5f),
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 1.0f, 50.0f);
    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f));
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f));
	glm::vec4 lp=glm::vec4(0.0f, 0.0f, -6.0f, -1.0f);
    sp->use();
	glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("color"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord"));
	glEnableVertexAttribArray(sp->a("texture"));
	
    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, boardVertices);
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, boardColors);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, boardNormals);
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, boardTexCoords);
	glVertexAttribPointer(sp->a("texture"), 1, GL_FLOAT, false, 0, boardTextures);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, blackTileTexture);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, whiteTileTexture);
	glUniform1i(sp->u("blackTile"), 0);
	glUniform1i(sp->u("whiteTile"), 1);
	
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	
    glDrawArrays(GL_TRIANGLES,0,BOARD_VERTEX_COUNT);
	
	glm::mat4 M2 = glm::translate(M,glm::vec3(0.0f,-1.0f,0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M2));
	
	glDrawArrays(GL_TRIANGLES,0,BOARD_VERTEX_COUNT);
	
    glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
	glDisableVertexAttribArray(sp->a("texture"));

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
