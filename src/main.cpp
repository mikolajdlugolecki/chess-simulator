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

#define ONE_TILE 66.6f

float speedX = 0;
float speedY = 0;
float aspectRatio = 1;
ShaderProgram *shaderProgram;
GLuint whiteTileTexture;
GLuint blackTileTexture;
GLuint bishopVAO, bishopVBO[3];
GLuint kingVAO, kingVBO[3];
GLuint knightVAO, knightVBO[3];
GLuint pawnVAO, pawnVBO[3];
GLuint queenVAO, queenVBO[3];
GLuint rookVAO, rookVBO[3];
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

void loadFigures()
{
	loadFigure("model/Bishop.obj", bishopVertexCount, &bishopVertices, &bishopNormals, &bishopTexCoords);
	loadFigure("model/King.obj", kingVertexCount, &kingVertices, &kingNormals, &kingTexCoords);
	loadFigure("model/Knight.obj", knightVertexCount, &knightVertices, &knightNormals, &knightTexCoords);
	loadFigure("model/Pawn.obj", pawnVertexCount, &pawnVertices, &pawnNormals, &pawnTexCoords);
	loadFigure("model/Queen.obj", queenVertexCount, &queenVertices, &queenNormals, &queenTexCoords);
	loadFigure("model/Rook.obj", rookVertexCount, &rookVertices, &rookNormals, &rookTexCoords);
}

void prepareFigure(GLuint &VAO, GLuint *VBO, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(float), normals->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexNormalsName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexNormalsName));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, texCoords->size() * sizeof(float), texCoords->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(shaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(shaderProgram->a(shaderVertexTexturingCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareFigures()
{
	prepareFigure(bishopVAO, bishopVBO, &bishopVertices, &bishopNormals, &bishopTexCoords);
	prepareFigure(kingVAO, kingVBO, &kingVertices, &kingNormals, &kingTexCoords);
	prepareFigure(knightVAO, knightVBO, &knightVertices, &knightNormals, &knightTexCoords);
	prepareFigure(pawnVAO, pawnVBO, &pawnVertices, &pawnNormals, &pawnTexCoords);
	prepareFigure(queenVAO, queenVBO, &queenVertices, &queenNormals, &queenTexCoords);
	prepareFigure(rookVAO, rookVBO, &rookVertices, &rookNormals, &rookTexCoords);
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
	loadFigures();
	prepareFigures();
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete shaderProgram;
	glDeleteTextures(1, &whiteTileTexture);
	glDeleteTextures(1, &blackTileTexture);
	glDeleteVertexArrays(1, &bishopVAO);
	glDeleteBuffers(3, bishopVBO);
	glDeleteVertexArrays(1, &kingVAO);
	glDeleteBuffers(3, kingVBO);
	glDeleteVertexArrays(1, &knightVAO);
	glDeleteBuffers(3, knightVBO);
	glDeleteVertexArrays(1, &pawnVAO);
	glDeleteBuffers(3, pawnVBO);
	glDeleteVertexArrays(1, &queenVAO);
	glDeleteBuffers(3, queenVBO);
	glDeleteVertexArrays(1, &rookVAO);
	glDeleteBuffers(3, rookVBO);
}

void drawBishop(glm::mat4 modelMatrix)
{
	glBindVertexArray(bishopVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, bishopVertexCount);
	glBindVertexArray(0);
}

void drawKing(glm::mat4 modelMatrix)
{
	glBindVertexArray(kingVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, kingVertexCount);
	glBindVertexArray(0);
}

void drawKnight(glm::mat4 modelMatrix)
{
	glBindVertexArray(knightVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, knightVertexCount);
	glBindVertexArray(0);
}

void drawPawn(glm::mat4 modelMatrix)
{
	glBindVertexArray(pawnVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);
	glBindVertexArray(0);
}

void drawQueen(glm::mat4 modelMatrix)
{
	glBindVertexArray(queenVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, queenVertexCount);
	glBindVertexArray(0);
}

void drawRook(glm::mat4 modelMatrix)
{
	glBindVertexArray(rookVAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, rookVertexCount);
	glBindVertexArray(0);
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

	glUniform1i(shaderProgram->u("useTexture"), 1);
	
    glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);
	
	glm::mat4 M2 = glm::translate(modelMatrix, glm::vec3(0.f, -0.5f, 0.f));
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(M2));
	
	glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);

	glm::mat4 spawn = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
	spawn = glm::scale(spawn, glm::vec3(0.015f, 0.015f, 0.015f));
	spawn = glm::translate(spawn, glm::vec3(3 * ONE_TILE, -3 * ONE_TILE, 0.f));
	
	glm::mat4 blackRook1 = glm::rotate(glm::translate(spawn, glm::vec3(0 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackKnight1 = glm::rotate(glm::translate(spawn, glm::vec3(-ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackBishop1 = glm::rotate(glm::translate(spawn, glm::vec3(-2 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackQueen = glm::rotate(glm::translate(spawn, glm::vec3(-3 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackKing = glm::rotate(glm::translate(spawn, glm::vec3(-4 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackBishop2 = glm::rotate(glm::translate(spawn, glm::vec3(-5 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackKnight2 = glm::rotate(glm::translate(spawn, glm::vec3(-6 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackRook2 = glm::rotate(glm::translate(spawn, glm::vec3(-7 * ONE_TILE, 0.f, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn1 = glm::rotate(glm::translate(spawn, glm::vec3(0 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn2 = glm::rotate(glm::translate(spawn, glm::vec3(-1 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn3 = glm::rotate(glm::translate(spawn, glm::vec3(-2 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn4 = glm::rotate(glm::translate(spawn, glm::vec3(-3 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn5 = glm::rotate(glm::translate(spawn, glm::vec3(-4 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn6 = glm::rotate(glm::translate(spawn, glm::vec3(-5 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn7 = glm::rotate(glm::translate(spawn, glm::vec3(-6 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 blackPawn8 = glm::rotate(glm::translate(spawn, glm::vec3(-7 * ONE_TILE, ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));

	drawRook(blackRook1);
	drawKnight(blackKnight1);
	drawBishop(blackBishop1);
	drawQueen(blackQueen);
	drawKing(blackKing);
	drawBishop(blackBishop2);
	drawKnight(blackKnight2);
	drawRook(blackRook2);
	drawPawn(blackPawn1);
	drawPawn(blackPawn2);
	drawPawn(blackPawn3);
	drawPawn(blackPawn4);
	drawPawn(blackPawn5);
	drawPawn(blackPawn6);
	drawPawn(blackPawn7);
	drawPawn(blackPawn8);

	glm::mat4 whiteRook1 = glm::translate(spawn, glm::vec3(0 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteKnight1 = glm::translate(spawn, glm::vec3(-1 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteBishop1 = glm::translate(spawn, glm::vec3(-2 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteQueen = glm::translate(spawn, glm::vec3(-3 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteKing = glm::translate(spawn, glm::vec3(-4 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteBishop2 = glm::translate(spawn, glm::vec3(-5 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteKnight2 = glm::translate(spawn, glm::vec3(-6 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whiteRook2 = glm::translate(spawn, glm::vec3(-7 * ONE_TILE, 7 * ONE_TILE, 0.f));
	glm::mat4 whitePawn1 = glm::translate(spawn, glm::vec3(0 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn2 = glm::translate(spawn, glm::vec3(-1 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn3 = glm::translate(spawn, glm::vec3(-2 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn4 = glm::translate(spawn, glm::vec3(-3 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn5 = glm::translate(spawn, glm::vec3(-4 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn6 = glm::translate(spawn, glm::vec3(-5 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn7 = glm::translate(spawn, glm::vec3(-6 * ONE_TILE, 6 * ONE_TILE, 0.f));
	glm::mat4 whitePawn8 = glm::translate(spawn, glm::vec3(-7 * ONE_TILE, 6 * ONE_TILE, 0.f));

	drawRook(whiteRook1);
	drawKnight(whiteKnight1);
	drawBishop(whiteBishop1);
	drawQueen(whiteQueen);
	drawKing(whiteKing);
	drawBishop(whiteBishop2);
	drawKnight(whiteKnight2);
	drawRook(whiteRook2);
	drawPawn(whitePawn1);
	drawPawn(whitePawn2);
	drawPawn(whitePawn3);
	drawPawn(whitePawn4);
	drawPawn(whitePawn5);
	drawPawn(whitePawn6);
	drawPawn(whitePawn7);
	drawPawn(whitePawn8);

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
