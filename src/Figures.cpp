#include "Figures.h"

float Figure::animationDuration = 0.01f;

Figure::Figure(int initPositionX, int initPositionZ, bool figureColor)
{
    this->positionX = initPositionX;
    this->positionZ = initPositionZ;
    this->color = figureColor;
    this->inGame = true;
    this->animating = false;
    this->capturedPiece = nullptr;
    this->captureHandled = false;
}

Figure::~Figure(void)
{
    this->capturedPiece = nullptr;
    delete this->capturedPiece;
}

void Figure::setPosition(int inputPositionX, int inputPositionZ)
{
    this->positionX = inputPositionX;
    this->positionZ = inputPositionZ;
}

bool Figure::onPosition(int inputPositionX, int inputPositionZ)
{
    if(this->positionX == inputPositionX && this->positionZ == inputPositionZ)
        return true;
    return false;
}

void Figure::startMove(int toX, int toZ)
{
    this->currentX = this->positionX;
    this->currentZ = this->positionZ;
    this->targetX = toX;
    this->targetZ = toZ;
    this->animating = true;
    this->animationTime = 0.0f;
}

void Figure::updateAnimation(float deltaTime)
{
    if(!this->animating)
        return;
    this->animationTime += deltaTime;
    float currentTime = this->animationTime / this->animationDuration;
    if(currentTime >= 1.0f){
        currentTime = 1.0f;
        this->animating = false;
        if(this->color == WHITE){
            this->positionX = -this->targetX;
            this->positionZ = -this->targetZ;
        }else{
            this->positionX = this->targetX;
            this->positionZ = this->targetZ;
        }
    }
    float interpX; 
    float interpZ;
    float interpY;
    if(currentTime < 0.33f){
        float liftT = currentTime / 0.33f;
        interpY = 0.0f + (1.0f - 0.0f) * liftT;
        interpX = 0;
        interpZ = 0;
    }else if(currentTime < 0.66f){
        float moveT = (currentTime - 0.33f) / 0.33f;
        if(this->color == BLACK){
            interpX = (this->currentX - this->targetX) * moveT;
            interpZ = (this->currentZ - this->targetZ) * moveT;
        }else{
            interpX = -(this->currentX + this->targetX) * moveT;
            interpZ = -(this->currentZ + this->targetZ) * moveT;
        }
        interpY = 1.f;
    }else{
        float fallT = (currentTime - 0.66f) / 0.34f;
        interpY = 1.0f + (0.0f - 1.0f) * fallT;
        if(this->color == BLACK){
            interpX = (this->currentX - this->targetX);
            interpZ = (this->currentZ - this->targetZ);
        }else{
            interpX = -(this->currentX + this->targetX);
            interpZ = -(this->currentZ + this->targetZ);
        }
        if(!this->captureHandled && this->capturedPiece != nullptr){
            this->capturedPiece->inGame = false;
            this->captureHandled = true;
        }
    }
    this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(-interpX * ONE_TILE, -interpZ * ONE_TILE, interpY * ONE_TILE));
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
