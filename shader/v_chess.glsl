#version 330

layout (location=0) in vec4 vertexCoordinates;
layout (location=2) in vec2 vertexTexturingCoordinates;
in float textureIndexIn;
out vec2 interpolatedTexturingCoordinates;
flat out int textureIndex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main(void){
    gl_Position = P * V * M * vertexCoordinates;
    interpolatedTexturingCoordinates = vertexTexturingCoordinates;
    textureIndex = int(textureIndexIn);
}
