#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Figure
{
public:
    Figure();
    Figure(int offsetX, int offsetZ);
    glm::mat4 modelMatrix;
    virtual ~Figure();
    int offsetX;
    int offsetZ;
    int currentX;
    int currentZ;
};

class Bishop : public Figure
{
public:
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
    static unsigned int vertexCount;
    static std::vector<float> vertices;
    static std::vector<float> normals;
    static std::vector<float> texCoords;
    static GLuint VAO;
    static GLuint VBO[3];
};

#endif
