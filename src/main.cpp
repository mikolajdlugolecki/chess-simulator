#define GLM_FORCE_RADIANS

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

std::vector<std::string> moveFromTile;
std::vector<std::string> moveToTile;

ShaderProgram *chessShaderProgram;

GLuint whiteTileTexture;
GLuint blackTileTexture;
GLuint whiteFigureTexture;
GLuint blackFigureTexture;

const char* shaderVertexCoordinatesName = "vertexCoordinates";
const char* shaderVertexColorsName = "vertexColors";
const char* shaderVertexNormalsName = "vertexNormals";
const char* shaderVertexTexturingCoordinatesName = "vertexTexturingCoordinates";

std::vector<Figure*> whiteFigures;
std::vector<Figure*> blackFigures;

glm::vec3 offset=glm::vec3(-0.5f,0.5f,0.f); //vector do przesuwania centrum planszy
glm::mat4 changeViewMatrix =glm::lookAt(glm::vec3(0.f, 10.f, -12.5f)+offset, glm::vec3(0.f, 0.f, 0.f)+offset, glm::vec3(0.f, 1.f, 0.f)); //domyslny widok (1)

enum whatToDraw
{
	CHESSBOARD = 0,
	CHESSBOARD_BASE,
	WHITE_FIGURE,
	BLACK_FIGURE
};

void readFile(void);
void initChessboard(void);
void loadFigure(const char* fileName, unsigned int &vertexCount, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords);
void prepareFigure(GLuint &VAO, GLuint *VBO, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords);
void initFigures(void);
void setupFigures(glm::mat4 modelMatrix);
void createFigures();
void drawFigure(Figure *figure);
void drawChessboard(glm::mat4 modelMatrix);
void makeMoves(void);
GLuint readTexture(const char* filename);
void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowResizeCallback(GLFWwindow* window, int width, int height);
void enableShaders(void);
void disableShaders(void);
void initOpenGLProgram(GLFWwindow* window);
void drawScene(GLFWwindow* window, float angle_x, float angle_y);
void freeOpenGLProgram(GLFWwindow* window);

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(errorCallback);
	if(!glfwInit()){
		std::cerr<<"Can't initialize GLFW."<<std::endl;
		return EXIT_FAILURE;
	}
	window = glfwCreateWindow(1000, 1000, "OpenGL", NULL, NULL);
	if(!window){
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	GLenum error;
	if((error = glewInit()) != GLEW_OK){ 
		std::cerr<<"Can't initialize GLEW: "<<glewGetErrorString(error)<<std::endl;
		return EXIT_FAILURE;
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
	return EXIT_SUCCESS;
}

void initChessboard(void)
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
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, filename);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS){
		if(key == GLFW_KEY_R)
			createFigures();
		if(key == GLFW_KEY_SPACE)
			makeMoves();
		if(key == GLFW_KEY_1){
			offset=glm::vec3(-0.5f,0.5f,0.f);
			changeViewMatrix =glm::lookAt(glm::vec3(0.f, 10.f, -12.5f)+offset, glm::vec3(0.f, 0.f, 0.f)+offset, glm::vec3(0.f, 1.f, 0.f));
		}
		if(key == GLFW_KEY_2){
			offset=glm::vec3(-0.5f,1.0f,-0.5f);
			changeViewMatrix =glm::lookAt(glm::vec3(0.f, 10.f, 12.5f)+offset, glm::vec3(0.f, 0.f, 0.f)+offset, glm::vec3(0.f, 1.f, 0.f));
		}
		if(key == GLFW_KEY_3){
			offset=glm::vec3(0.f,0.f,-0.5f);
			changeViewMatrix = glm::lookAt(glm::vec3(10.f, 10.f, 0.f)+offset, glm::vec3(0.f, 0.f, 0.f)+offset, glm::vec3(0.f, 1.f, 0.f));
		}
		if(key == GLFW_KEY_4){
			offset=glm::vec3(-1.f,0.f,-0.5f);
			changeViewMatrix = glm::lookAt(glm::vec3(-10.f, 10.f, 0.f)+offset, glm::vec3(0.f, 0.f, 0.f)+offset, glm::vec3(0.f, 1.f, 0.f));
		}
	}
    if(action == GLFW_RELEASE){
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) 
{
    if(height == 0)
		return;
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void enableShaders(void)
{
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexCoordinatesName));
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexColorsName));
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexNormalsName));
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexTexturingCoordinatesName));
	glEnableVertexAttribArray(chessShaderProgram->a("textureIndexIn"));

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, blackTileTexture);
	glUniform1i(chessShaderProgram->u("blackTileTexture"), 0);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, whiteTileTexture);
	glUniform1i(chessShaderProgram->u("whiteTileTexture"), 1);
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, blackFigureTexture);
	glUniform1i(chessShaderProgram->u("blackFigureTexture"), 2);
	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, whiteFigureTexture);
	glUniform1i(chessShaderProgram->u("whiteFigureTexture"), 3);
}

void disableShaders(void)
{
	glDisableVertexAttribArray(chessShaderProgram->a(shaderVertexCoordinatesName));
	glDisableVertexAttribArray(chessShaderProgram->a(shaderVertexColorsName));
	glDisableVertexAttribArray(chessShaderProgram->a(shaderVertexNormalsName));
	glDisableVertexAttribArray(chessShaderProgram->a(shaderVertexTexturingCoordinatesName));
	glDisableVertexAttribArray(chessShaderProgram->a("textureIndexIn"));
}

void prepareFigure(GLuint &VAO, GLuint *VBO, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texCoords)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(float), normals->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexNormalsName), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexNormalsName));
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, texCoords->size() * sizeof(float), texCoords->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(chessShaderProgram->a(shaderVertexTexturingCoordinatesName));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setupFigures(glm::mat4 modelMatrix)
{
	for(Figure* whiteFigure : whiteFigures)
		whiteFigure->modelMatrix = glm::translate(modelMatrix, glm::vec3(whiteFigure->positionX * ONE_TILE, whiteFigure->positionZ * ONE_TILE, 0.f));
	for(Figure* blackFigure : blackFigures)
		blackFigure->modelMatrix = glm::rotate(glm::translate(modelMatrix, glm::vec3(blackFigure->positionX * ONE_TILE, blackFigure->positionZ * ONE_TILE, 0.f)), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
}

void createFigures()
{
	for(Figure* whiteFigure : whiteFigures)
		delete whiteFigure;
	for(Figure* blackFigure : blackFigures)
		delete blackFigure;
	whiteFigures.clear();
	blackFigures.clear();
	whiteFigures.push_back(new Rook(0, 0));
	whiteFigures.push_back(new Knight(1, 0));
	whiteFigures.push_back(new Bishop(2, 0));
	whiteFigures.push_back(new Queen(3, 0));
	whiteFigures.push_back(new King(4, 0));
	whiteFigures.push_back(new Bishop(5, 0));
	whiteFigures.push_back(new Knight(6, 0));
	whiteFigures.push_back(new Rook(7, 0));
	blackFigures.push_back(new Rook(0, 7));
	blackFigures.push_back(new Knight(1, 7));
	blackFigures.push_back(new Bishop(2, 7));
	blackFigures.push_back(new Queen(3, 7));
	blackFigures.push_back(new King(4, 7));
	blackFigures.push_back(new Bishop(5, 7));
	blackFigures.push_back(new Knight(6, 7));
	blackFigures.push_back(new Rook(7, 7));
	for(int i=0; i<8; i++){
		whiteFigures.push_back(new Pawn(i, 1));
		blackFigures.push_back(new Pawn(i, 6));
	}
}

void readFile(void)
{
	std::string line = "";
	std::fstream file("game", std::ios::in);
	if(!file.is_open())
		std::cerr<<"Error: Cannot open a file"<<std::endl;
	std::string move;
	while(getline(file, line)){
		move = "";
		move += line[0];
		move += line[1];
		moveFromTile.push_back(move);
		move = "";
		move += line[3];
		move += line[4];
		moveToTile.push_back(move);
	}
	file.close();
}

void makeMoves(void)
{
	int sourceX, sourceZ, destinationX, destinationZ;
	for(int i=0; i<moveFromTile.size(); i++){
		sourceX = boardMap[moveFromTile[i][0]];
		sourceZ = int(moveFromTile[i][1]) - 1 - 48;
		destinationX = boardMap[moveToTile[i][0]];
		destinationZ = int(moveToTile[i][1]) - 1 - 48;
		std::vector<Figure*> potentialFiguresToMove;
		std::vector<Figure*> potentialFiguresToDelete;
		if(i % 2 == 0){
			potentialFiguresToMove = whiteFigures;
			potentialFiguresToDelete = blackFigures;
		}else{
			potentialFiguresToMove = blackFigures;
			potentialFiguresToDelete = whiteFigures;
		}
		for(Figure* figure : potentialFiguresToMove)
			if(figure->onPosition(sourceX, sourceZ)){
				figure->setPosition(destinationX, destinationZ);
				for(Figure* figure : potentialFiguresToDelete)
					if(figure->onPosition(destinationX, destinationZ)){
						figure->inGame = false;
						break;
					}
				break;
			}
	}
}

void initOpenGLProgram(GLFWwindow* window)
{
	readFile();
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	chessShaderProgram = new ShaderProgram("shader/vertex-shader.glsl", NULL, "shader/fragment-shader.glsl");
	whiteTileTexture = readTexture("texture/white-tile.png");
	blackTileTexture = readTexture("texture/black-tile.png");
	whiteFigureTexture = readTexture("texture/light-wood.png");
	blackFigureTexture = readTexture("texture/dark-wood.png");
	initChessboard();
	initFigures();
	makeMoves();
}

void freeOpenGLProgram(GLFWwindow* window)
{
	delete chessShaderProgram;
	glDeleteTextures(1, &whiteTileTexture);
	glDeleteTextures(1, &blackTileTexture);
	glDeleteTextures(1, &whiteFigureTexture);
	glDeleteTextures(1, &blackFigureTexture);
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
	for(Figure* whiteFigure : whiteFigures)
		delete whiteFigure;
	for(Figure* blackFigure : blackFigures)
		delete blackFigure;
	whiteFigures.clear();
	blackFigures.clear();
}

void drawFigure(Figure *figure)
{
	if(!figure->inGame)
		return;
	GLuint VAO;
	unsigned int vertexCount;
	if(dynamic_cast<Bishop*>(figure)){
		VAO = Bishop::VAO;
		vertexCount = Bishop::vertexCount;
	}else if(dynamic_cast<King*>(figure)){
		VAO = King::VAO;
		vertexCount = King::vertexCount;
	}else if(dynamic_cast<Knight*>(figure)){
		VAO = Knight::VAO;
		vertexCount = Knight::vertexCount;
	}else if(dynamic_cast<Pawn*>(figure)){
		VAO = Pawn::VAO;
		vertexCount = Pawn::vertexCount;
	}else if(dynamic_cast<Queen*>(figure)){
		VAO = Queen::VAO;
		vertexCount = Queen::vertexCount;
	}else if(dynamic_cast<Rook*>(figure)){
		VAO = Rook::VAO;
		vertexCount = Rook::vertexCount;
	}
	glBindVertexArray(VAO);
	glUniformMatrix4fv(chessShaderProgram->u("M"), 1, false, glm::value_ptr(figure->modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

void drawChessboard(glm::mat4 modelMatrix)
{
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, false, 0, boardVertices);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexColorsName), 4, GL_FLOAT, false, 0, boardColors);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexNormalsName), 4, GL_FLOAT, false, 0, boardNormals);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, false, 0, boardTexCoords);
	glVertexAttribPointer(chessShaderProgram->a("textureIndexIn"), 1, GL_FLOAT, false, 0, boardTextures);
	
	glUniformMatrix4fv(chessShaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));

	glUniform1i(chessShaderProgram->u("whatToDraw"), CHESSBOARD);
	
    glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);
	
	glm::mat4 model2Matrix = glm::translate(modelMatrix, glm::vec3(0.f, -0.5f, 0.f));
	glUniformMatrix4fv(chessShaderProgram->u("M"), 1, false, glm::value_ptr(model2Matrix));
	
	glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);

	glVertexAttribPointer(chessShaderProgram->a(shaderVertexCoordinatesName), 4, GL_FLOAT, false, 0, boardBaseVertices);
	glVertexAttribPointer(chessShaderProgram->a(shaderVertexTexturingCoordinatesName), 2, GL_FLOAT, false, 0, boardBaseTexCoords);

	glUniform1i(chessShaderProgram->u("whatToDraw"), CHESSBOARD_BASE);

	glm::mat4 baseModelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	baseModelMatrix = glm::scale(baseModelMatrix, glm::vec3(4.5f, 1.99f, 4.5f));
	baseModelMatrix = glm::translate(baseModelMatrix, glm::vec3(0.11f, -0.126f, -0.11f));

	glUniformMatrix4fv(chessShaderProgram->u("M"), 1, false, glm::value_ptr(baseModelMatrix));

	glDrawArrays(GL_TRIANGLES, 0, boardBaseVertexCount);
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewMatrix = changeViewMatrix;
    glm::mat4 projectionMatrix = glm::perspective(45.f * PI / 180.f, aspectRatio, 1.f, 50.f);
    glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::rotate(modelMatrix, angle_y, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, angle_x, glm::vec3(0.f, 1.f, 0.f));
	glm::vec4 lightPosition = glm::vec4(0.f, 0.f, -6.f, -1.f);

    chessShaderProgram->use();

	enableShaders();

	glUniformMatrix4fv(chessShaderProgram->u("P"), 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(chessShaderProgram->u("V"), 1, false, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(chessShaderProgram->u("M"), 1, false, glm::value_ptr(modelMatrix));

	drawChessboard(modelMatrix);	
	
	glm::mat4 spawn = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
	spawn = glm::scale(spawn, glm::vec3(0.015f, 0.015f, 0.015f));
	spawn = glm::translate(spawn, glm::vec3(3 * ONE_TILE, 4 * ONE_TILE, 0.f));
	setupFigures(spawn);
	
	glUniform1i(chessShaderProgram->u("whatToDraw"), WHITE_FIGURE);
	for(Figure* whiteFigure : whiteFigures)
		drawFigure(whiteFigure);

	glUniform1i(chessShaderProgram->u("whatToDraw"), BLACK_FIGURE);
	for(Figure* blackFigure : blackFigures)
		drawFigure(blackFigure);

	disableShaders();
	
    glfwSwapBuffers(window);
}

void initFigures(void)
{
	loadFigure("model/Bishop.obj", Bishop::vertexCount, &Bishop::vertices, &Bishop::normals, &Bishop::texCoords);
	loadFigure("model/King.obj", King::vertexCount, &King::vertices, &King::normals, &King::texCoords);
	loadFigure("model/Knight.obj", Knight::vertexCount, &Knight::vertices, &Knight::normals, &Knight::texCoords);
	loadFigure("model/Pawn.obj", Pawn::vertexCount, &Pawn::vertices, &Pawn::normals, &Pawn::texCoords);
	loadFigure("model/Queen.obj", Queen::vertexCount, &Queen::vertices, &Queen::normals, &Queen::texCoords);
	loadFigure("model/Rook.obj", Rook::vertexCount, &Rook::vertices, &Rook::normals, &Rook::texCoords);
	prepareFigure(Bishop::VAO, Bishop::VBO, &Bishop::vertices, &Bishop::normals, &Bishop::texCoords);
	prepareFigure(King::VAO, King::VBO, &King::vertices, &King::normals, &King::texCoords);
	prepareFigure(Knight::VAO, Knight::VBO, &Knight::vertices, &Knight::normals, &Knight::texCoords);
	prepareFigure(Pawn::VAO, Pawn::VBO, &Pawn::vertices, &Pawn::normals, &Pawn::texCoords);
	prepareFigure(Queen::VAO, Queen::VBO, &Queen::vertices, &Queen::normals, &Queen::texCoords);
	prepareFigure(Rook::VAO, Rook::VBO, &Rook::vertices, &Rook::normals, &Rook::texCoords);
	createFigures();
}
