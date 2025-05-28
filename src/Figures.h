#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#define ONE_TILE 66.6f

enum figureColor
{
    WHITE = 0,
    BLACK
};

class Figure
{
public:
    Figure(int initPositionX, int initPositionZ, bool color);
    virtual ~Figure(void);
    glm::mat4 modelMatrix;
    int positionX;
    int positionZ;
    bool color;
    bool inGame;
    bool animating;
    float currentX;
    float currentZ;
    float targetX; 
    float targetZ;
    float animationTime;
    static float animationDuration;
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
