#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "constants.h"
#include "lodepng.h"
#include "OBJ_Loader.h"
#include "ShaderProgram.h"
#include "Board.h"
#include "Figures.h"

#define ONE_TILE 66.6f

std::string from[1000];
std::string to[1000];
float speedX = 0;
float speedY = 0;
float aspectRatio = 1;
ShaderProgram *shaderProgram;
GLuint whiteTileTexture;
GLuint blackTileTexture;
const char* shaderVertexCoordinatesName = "vertexCoordinates";
const char* shaderVertexColorsName = "vertexColors";
const char* shaderVertexNormalsName = "vertexNormals";
const char* shaderVertexTexturingCoordinatesName = "vertexTexturingCoordinates";
Rook* whiteRook1 = new Rook(0, 0);
Knight* whiteKnight1 = new Knight(-1, 0);
Bishop* whiteBishop1 = new Bishop(-2, 0);
Queen* whiteQueen = new Queen(-3, 0);
King* whiteKing = new King(-4, 0);
Bishop* whiteBishop2 = new Bishop(-5, 0);
Knight* whiteKnight2 = new Knight(-6, 0);
Rook* whiteRook2 = new Rook(-7, 0);

Rook* blackRook1 = new Rook(0, 7);
Knight* blackKnight1 = new Knight(-1, 7);
Bishop* blackBishop1 = new Bishop(-2, 7);
Queen* blackQueen = new Queen(-3, 7);
King* blackKing = new King(-4, 7);
Bishop* blackBishop2 = new Bishop(-5, 7);
Knight* blackKnight2 = new Knight(-6, 7);
Rook* blackRook2 = new Rook(-7, 7);
std::vector<Figure*> whiteFigures;
std::vector<Figure*> blackFigures;

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

void loadFigure(const char* fileName, unsigned int &vertexCount, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords)
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
        // if(key == GLFW_KEY_LEFT) 
		// 	speedX = -PI / 2;
        // if(key == GLFW_KEY_RIGHT)
		// 	speedX = PI / 2;
        // if(key == GLFW_KEY_UP)
		// 	speedY = PI / 2;
        // if(key == GLFW_KEY_DOWN)
		// 	speedY = -PI / 2;
		if(key == GLFW_KEY_LEFT) 
			whiteRook1->currentX++;
        if(key == GLFW_KEY_RIGHT)
			whiteRook1->currentX--;
        if(key == GLFW_KEY_UP)
			whiteRook1->currentZ--;
        if(key == GLFW_KEY_DOWN)
			whiteRook1->currentZ++;
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
	loadFigure("model/Bishop.obj", Bishop::vertexCount, &Bishop::vertices, &Bishop::normals, &Bishop::texCoords);
	loadFigure("model/King.obj", King::vertexCount, &King::vertices, &King::normals, &King::texCoords);
	loadFigure("model/Knight.obj", Knight::vertexCount, &Knight::vertices, &Knight::normals, &Knight::texCoords);
	loadFigure("model/Pawn.obj", Pawn::vertexCount, &Pawn::vertices, &Pawn::normals, &Pawn::texCoords);
	loadFigure("model/Queen.obj", Queen::vertexCount, &Queen::vertices, &Queen::normals, &Queen::texCoords);
	loadFigure("model/Rook.obj", Rook::vertexCount, &Rook::vertices, &Rook::normals, &Rook::texCoords);
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
	prepareFigure(Bishop::VAO, Bishop::VBO, &Bishop::vertices, &Bishop::normals, &Bishop::texCoords);
	prepareFigure(King::VAO, King::VBO, &King::vertices, &King::normals, &King::texCoords);
	prepareFigure(Knight::VAO, Knight::VBO, &Knight::vertices, &Knight::normals, &Knight::texCoords);
	prepareFigure(Pawn::VAO, Pawn::VBO, &Pawn::vertices, &Pawn::normals, &Pawn::texCoords);
	prepareFigure(Queen::VAO, Queen::VBO, &Queen::vertices, &Queen::normals, &Queen::texCoords);
	prepareFigure(Rook::VAO, Rook::VBO, &Rook::vertices, &Rook::normals, &Rook::texCoords);
}

void setupFigures(glm::mat4 modelMatrix)
{
	for(Figure* whiteFigure : whiteFigures)
		whiteFigure->modelMatrix = glm::rotate(glm::translate(modelMatrix, glm::vec3((whiteFigure->initX + whiteFigure->currentX) * ONE_TILE, (whiteFigure->initZ + whiteFigure->currentZ) * ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	for(Figure* blackFigure : blackFigures)
		blackFigure->modelMatrix = glm::translate(modelMatrix, glm::vec3((blackFigure->initX + blackFigure->currentX) * ONE_TILE, (blackFigure->initZ + blackFigure->currentZ) * ONE_TILE, 0.f));
}

void initOpenGLProgram(GLFWwindow* window)
{
	
	std::string move;
	std::ifstream readFile("src/moves.txt");
	if(!readFile.is_open()){
		std::cerr << "Error: Cannot open a file" << std::endl;
	}
	int curr_mv=0,n=0;
	while(getline(readFile,move)){
		from[curr_mv] = "";
		from[curr_mv]+= move[0];
		from[curr_mv]+= move[1];
		to[curr_mv] = "";
		to[curr_mv]+= move[3];
		to[curr_mv]+= move[4];
		curr_mv++;
		n++;
	}
	readFile.close();
	int i=0;
	while(i<n){
		if(i%2==0){
			std::cout << "Ruch białych z "<< from[i] << " na "<< to[i] << std::endl;
		}
		else std::cout << "Ruch czarnych z "<< from[i] << " na "<< to[i] << std::endl;
		i++;
	}
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
	whiteFigures.push_back(whiteRook1);
	whiteFigures.push_back(whiteKnight1);
	whiteFigures.push_back(whiteBishop1);
	whiteFigures.push_back(whiteQueen);
	whiteFigures.push_back(whiteKing);
	whiteFigures.push_back(whiteBishop2);
	whiteFigures.push_back(whiteKnight2);
	whiteFigures.push_back(whiteRook2);
	blackFigures.push_back(blackRook1);
	blackFigures.push_back(blackKnight1);
	blackFigures.push_back(blackBishop1);
	blackFigures.push_back(blackQueen);
	blackFigures.push_back(blackKing);
	blackFigures.push_back(blackBishop2);
	blackFigures.push_back(blackKnight2);
	blackFigures.push_back(blackRook2);
	for(int i=0; i<8; i++){
		whiteFigures.push_back(new Pawn(-i, 1));
		blackFigures.push_back(new Pawn(-i, 6));
	}
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete shaderProgram;
	glDeleteTextures(1, &whiteTileTexture);
	glDeleteTextures(1, &blackTileTexture);
	glDeleteVertexArrays(1, &Bishop::VAO);
	glDeleteBuffers(3, Bishop::VBO);
	glDeleteVertexArrays(1, &King::VAO);
	glDeleteBuffers(3, King::VBO);
	glDeleteVertexArrays(1, &Knight::VAO);
	glDeleteBuffers(3, Knight::VBO);
	glDeleteVertexArrays(1, &Pawn::VAO);
	glDeleteBuffers(3, Pawn::VBO);
	glDeleteVertexArrays(1, &Queen::VAO);
	glDeleteBuffers(3, Queen::VBO);
	glDeleteVertexArrays(1, &Rook::VAO);
	glDeleteBuffers(3, Rook::VBO);
	for(Figure* figure : whiteFigures)
		delete figure;
	whiteFigures.clear();
}

void draw(Figure *figure)
{
	GLuint VAO;
	unsigned int vertexCount;
	if(dynamic_cast<Bishop*>(figure)){
		VAO = Bishop::VAO;
		vertexCount = Bishop::vertexCount;
	}
	else if(dynamic_cast<King*>(figure)){
		VAO = King::VAO;
		vertexCount = King::vertexCount;
	}
		else if(dynamic_cast<Knight*>(figure)){
		VAO = Knight::VAO;
		vertexCount = Knight::vertexCount;
	}
		else if(dynamic_cast<Pawn*>(figure)){
		VAO = Pawn::VAO;
		vertexCount = Pawn::vertexCount;
	}
		else if(dynamic_cast<Queen*>(figure)){
		VAO = Queen::VAO;
		vertexCount = Queen::vertexCount;
	}
		else if(dynamic_cast<Rook*>(figure)){
		VAO = Rook::VAO;
		vertexCount = Rook::vertexCount;
	}
	glBindVertexArray(VAO);
	glUniformMatrix4fv(shaderProgram->u("M"), 1, false, glm::value_ptr(figure->modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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
	setupFigures(spawn);

	for(Figure* whiteFigure : whiteFigures)
		draw(whiteFigure);
	for(Figure* blackFigure : blackFigures)
		draw(blackFigure);

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
