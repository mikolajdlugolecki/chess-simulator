#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Figure
{
public:
    Figure();
    Figure(glm::mat4 matrix);
    glm::mat4 modelMatrix;
    virtual ~Figure();
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
