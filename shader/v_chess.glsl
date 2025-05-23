#version 330

layout (location=0) in vec4 vertexCoordinates;
layout (location=2) in vec2 vertexTexturingCoordinates;
layout (location=3) in vec4 vertexColors;
in float textureIndexIn;
out vec2 interpolatedTexturingCoordinates;
out vec4 interpolatedColors;
flat out int textureIndex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform int useTexture;

void main(void){
    gl_Position = P * V * M * vertexCoordinates;
    interpolatedTexturingCoordinates = vertexTexturingCoordinates;
    interpolatedColors = vertexColors;
    textureIndex = int(textureIndexIn);
}
