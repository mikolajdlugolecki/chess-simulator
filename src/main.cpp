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
#include "OBJ_Loader.h"
#include "ShaderProgram.h"
#include "Board.h"
#include "Figures.h"

float speedX = 0;
float speedY = 0;
float aspectRatio = 1;
ShaderProgram *shaderProgram;
GLuint whiteTileTexture;
GLuint blackTileTexture;
GLuint pawnVAO, pawnVBO[3];
const char* shaderVertexCoordinatesName = "vertexCoordinates";
const char* shaderVertexColorsName = "vertexColors";
const char* shaderVertexNormalsName = "vertexNormals";
const char* shaderVertexTexturingCoordinatesName = "vertexTexturingCoordinates";

void genereteBoard(void)
{
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

void loadFigure(const char* fileName, int &vertexCount, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(fileName);
	if(!loadout)
		return;
	for(int i=0; i<loader.LoadedMeshes.size(); i++){
		objl::Mesh currentMesh = loader.LoadedMeshes[i];
		for(int j=0; j<currentMesh.Vertices.size(); j++){
			vertices->push_back(currentMesh.Vertices[j].Position.X);
			vertices->push_back(currentMesh.Vertices[j].Position.Y);
			vertices->push_back(currentMesh.Vertices[j].Position.Z);
			vertices->push_back(1.f);
			normals->push_back(currentMesh.Vertices[j].Normal.X);
			normals->push_back(currentMesh.Vertices[j].Normal.Y);
			normals->push_back(currentMesh.Vertices[j].Normal.Z);
			normals->push_back(0.f);
			texCoords->push_back(currentMesh.Vertices[j].TextureCoordinate.X);
			texCoords->push_back(currentMesh.Vertices[j].TextureCoordinate.Y);
		}
	}
	vertexCount = vertices->size() / 4;
}

GLuint readTexture(const char* filename)
{
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
			speedX = -PI / 2;
        if(key == GLFW_KEY_RIGHT)
			speedX = PI / 2;
        if(key == GLFW_KEY_UP)
			speedY = PI / 2;
        if(key == GLFW_KEY_DOWN)
			speedY = -PI / 2;
    }
    if(action == GLFW_RELEASE){
        if(key == GLFW_KEY_LEFT)
			speedX = 0;
        if(key == GLFW_KEY_RIGHT)
			speedX = 0;
        if(key == GLFW_KEY_UP)
			speedY = 0;
        if(key == GLFW_KEY_DOWN)
			speedY = 0;
    }
}

void windowResizeCallback(GLFWwindow* window, int width, int height) 
{
    if(height == 0)
		return;
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	shaderProgram = new ShaderProgram("shader/v_chess.glsl", NULL, "shader/f_chess.glsl");
	whiteTileTexture = readTexture("texture/white-tile.png");
	blackTileTexture = readTexture("texture/black-tile.png");
	genereteBoard();
	loadFigure("model/Bishop.obj", bishopVertexCount, &bishopVertices, &bishopNormals, &bishopTexCoords);
	//loadFigure("model/King.obj", kingVertexCount, &kingVertices, &kingNormals, &kingTexCoords);
	loadFigure("model/Knight.obj", knightVertexCount, &knightVertices, &knightNormals, &knightTexCoords);
	loadFigure("model/Pawn.obj", pawnVertexCount, &pawnVertices, &pawnNormals, &pawnTexCoords);
	loadFigure("model/Queen.obj", queenVertexCount, &queenVertices, &queenNormals, &queenTexCoords);
	//loadFigure("model/Rook.obj", rookVertexCount, &rookVertices, &rookNormals, &rookTexCoords);
	glGenVertexArrays(1, &pawnVAO);
	glGenBuffers(3, pawnVBO);
	glBindVertexArray(pawnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pawnVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pawnVertices.size() * sizeof(float), pawnVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, pawnVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, pawnNormals.size() * sizeof(float), pawnNormals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexNormalsName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexNormalsName));
	glBindBuffer(GL_ARRAY_BUFFER, pawnVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, pawnTexCoords.size() * sizeof(float), pawnTexCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexTexturingCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete shaderProgram;
	glDeleteTextures(1, &whiteTileTexture);
	glDeleteTextures(1, &blackTileTexture);
	glDeleteVertexArrays(1, &pawnVAO);
	glDeleteBuffers(3, pawnVBO);
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.f, 5.f, -12.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 projectionMatrix = glm::perspective(50.f * PI / 180.f, aspectRatio, 1.f, 50.f);
    glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::rotate(modelMatrix, angle_y, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, angle_x, glm::vec3(0.f, 1.f, 0.f));
	glm::vec4 lightPosition = glm::vec4(0.f, 0.f, -6.f, -1.f);
    shaderProgram->use();
	glUniformMatrix4fv(shaderProgram->u("P"), 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shaderProgram->u("V"), 1, false, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));

	glEnableVertexAttribArray(shaderProgram->a(shaderVertexCoordinatesName));
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexColorsName));
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexNormalsName));
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexTexturingCoordinatesName));
	glEnableVertexAttribArray(shaderProgram->a("textureIndexIn"));
	
    glVertexAttribPointer(shaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, false, 0, boardVertices);
	glVertexAttribPointer(shaderProgram->a(shaderVertexColorsName), 4, GL_FLOAT, false, 0, boardColors);
	glVertexAttribPointer(shaderProgram->a(shaderVertexNormalsName), 4, GL_FLOAT, false, 0, boardNormals);
	glVertexAttribPointer(shaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, false, 0, boardTexCoords);
	glVertexAttribPointer(shaderProgram->a("textureIndexIn"), 1, GL_FLOAT, false, 0, boardTextures);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, blackTileTexture);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, whiteTileTexture);
	glUniform1i(shaderProgram->u("blackTile"), 0);
	glUniform1i(shaderProgram->u("whiteTile"), 1);
	
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	
    glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);
	
	glm::mat4 M2 = glm::translate(modelMatrix, glm::vec3(0.f, -0.5f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(M2));
	
	glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);

	glm::mat4 blackPawn1 = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
	blackPawn1 = glm::scale(blackPawn1, glm::vec3(0.015f, 0.015f, 0.015f));
	glm::mat4 whitePawn1 = glm::translate(blackPawn1, glm::vec3(-4 * 66.6f, -2 * 66.6f, 0.f));
	blackPawn1 = glm::translate(blackPawn1, glm::vec3(-4 * 66.6f, 3 * 66.6f, 0.f));

	glBindVertexArray(pawnVAO);

	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn1));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn2 = glm::translate(blackPawn1, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn2));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn3 = glm::translate(blackPawn2, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn3));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn4 = glm::translate(blackPawn3, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn4));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn5 = glm::translate(blackPawn4, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn5));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn6 = glm::translate(blackPawn5, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn6));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn7 = glm::translate(blackPawn6, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn7));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 blackPawn8 = glm::translate(blackPawn7, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(blackPawn8));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	
	
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false,glm::value_ptr(whitePawn1));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn2 = glm::translate(whitePawn1, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn2));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn3 = glm::translate(whitePawn2, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn3));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn4 = glm::translate(whitePawn3, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn4));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn5 = glm::translate(whitePawn4, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn5));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn6 = glm::translate(whitePawn5, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn6));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn7 = glm::translate(whitePawn6, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn7));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glm::mat4 whitePawn8 = glm::translate(whitePawn7, glm::vec3(66.6f, 0.f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(whitePawn8));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);

	glBindVertexArray(0);

    glDisableVertexAttribArray(shaderProgram->a(shaderVertexCoordinatesName));
	glDisableVertexAttribArray(shaderProgram->a(shaderVertexColorsName));
	glDisableVertexAttribArray(shaderProgram->a(shaderVertexNormalsName));
	glDisableVertexAttribArray(shaderProgram->a(shaderVertexTexturingCoordinatesName));
	glDisableVertexAttribArray(shaderProgram->a("textureIndexIn"));

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
	float angleX = 0;
	float angleY = 0;
	glfwSetTime(0);
	
	while(!glfwWindowShouldClose(window)){
        angleX += speedX * glfwGetTime();
		angleY += speedY * glfwGetTime();
        glfwSetTime(0);
		drawScene(window, angleX, angleY);
		glfwPollEvents();
	}
	freeOpenGLProgram(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
