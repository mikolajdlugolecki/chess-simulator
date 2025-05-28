#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#define ONE_TILE 66.6f

enum figureColor
{
    BLACK = 0,
    WHITE
};

class Figure
{
public:
    Figure(int initPositionX, int initPositionZ, bool color);
    virtual ~Figure();
    glm::mat4 modelMatrix;
    int positionX;
    int positionZ;
    bool color;
    bool inGame;
    bool animating = false;
    float currentX;
    float currentY = 0.f;
    float currentZ;
    float targetX; 
    float targetY;
    float targetZ;
    float animationTime = 0.0f;
    float animationDuration = 0.01f;
    void setPosition(int inputPositionX, int inputPositionZ);
    bool onPosition(int inputPositionX, int inputPositionZ);
    void startMove(int toX, int toZ);
    void updateAnimation(float deltaTime);
};

class Bishop : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

class King : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

class Knight : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

class Pawn : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

class Queen : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

class Rook : public Figure
{
public:
    using Figure::Figure;
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

#endif
