#version 330

layout (location=0) in vec4 vertexCoordinates;
layout (location=1) in vec4 vertexNormals;
layout (location=2) in vec2 vertexTexturingCoordinates;
layout (location=3) in vec4 vertexColors;
in float textureIndexIn;
out vec2 interpolatedTexturingCoordinates;
out vec4 interpolatedColors;
out vec4 l;
out vec4 n;
out vec4 v;
flat out int textureIndex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
// uniform vec4 lp;

void main(void){
    gl_Position = P * V * M * vertexCoordinates;
    interpolatedTexturingCoordinates = vertexTexturingCoordinates;
    interpolatedColors = vertexColors;
    textureIndex = int(textureIndexIn);
    vec4 lp = vec4(0.f, 0.f, -6.f, -1.f);
    l = normalize(V * lp - V * M * vertexCoordinates);
    n = normalize(V * M * vertexNormals);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertexCoordinates);
}
