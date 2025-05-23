#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Figure
{
public:
    Figure();
    Figure(int initPositionX, int initPositionZ);
    glm::mat4 modelMatrix;
    virtual ~Figure();
    int positionX;
    int positionZ;
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
