#version 330

layout (location=0) in vec4 vertexCoordinates;
layout (location=1) in vec4 vertexNormals;
layout (location=2) in vec2 vertexTexturingCoordinates;
layout (location=3) in vec4 vertexColors;
in float textureIndexIn;
out vec2 interpolatedTexturingCoordinates;
out vec4 interpolatedColors;
out vec4 l1;
out vec4 l2;
out vec4 n;
out vec4 v;
flat out int textureIndex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp1;
uniform vec4 lp2;

void main(void){
    gl_Position = P * V * M * vertexCoordinates;
    interpolatedTexturingCoordinates = vertexTexturingCoordinates;
    interpolatedColors = vertexColors;
    textureIndex = int(textureIndexIn);
    l1 = normalize(V * lp1 - V * M * vertexCoordinates);
    l2 = normalize(V * lp2 - V * M * vertexCoordinates);
    n = normalize(V * M * vertexNormals);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertexCoordinates);
}
