#include "Figures.h"

Figure::Figure()
{
    this->modelMatrix = glm::mat4(1.f);
}

Figure::Figure(int initX, int initZ)
{
    this->initX = initX;
    this->initZ = initZ;
    this->currentX = 0;
    this->currentZ = 0;
}

Figure::~Figure()
{

}

unsigned int Bishop::vertexCount = 0;
unsigned int King::vertexCount = 0;
unsigned int Knight::vertexCount = 0;
unsigned int Pawn::vertexCount = 0;
unsigned int Queen::vertexCount = 0;
unsigned int Rook::vertexCount = 0;
std::vector<float> Bishop::vertices = {};
std::vector<float> Bishop::normals = {};
std::vector<float> Bishop::texCoords = {};
std::vector<float> King::vertices = {};
std::vector<float> King::normals = {};
std::vector<float> King::texCoords = {};
std::vector<float> Knight::vertices = {};
std::vector<float> Knight::normals = {};
std::vector<float> Knight::texCoords = {};
std::vector<float> Pawn::vertices = {};
std::vector<float> Pawn::normals = {};
std::vector<float> Pawn::texCoords = {};
std::vector<float> Queen::vertices = {};
std::vector<float> Queen::normals = {};
std::vector<float> Queen::texCoords = {};
std::vector<float> Rook::vertices = {};
std::vector<float> Rook::normals = {};
std::vector<float> Rook::texCoords = {};
GLuint Bishop::VAO = 0;
GLuint Bishop::VBO[3] = {0, 0, 0};
GLuint King::VAO = 0;
GLuint King::VBO[3] = {0, 0, 0};
GLuint Knight::VAO = 0;
GLuint Knight::VBO[3] = {0, 0, 0};
GLuint Pawn::VAO = 0;
GLuint Pawn::VBO[3] = {0, 0, 0};
GLuint Queen::VAO = 0;
GLuint Queen::VBO[3] = {0, 0, 0};
GLuint Rook::VAO = 0;
GLuint Rook::VBO[3] = {0, 0, 0};