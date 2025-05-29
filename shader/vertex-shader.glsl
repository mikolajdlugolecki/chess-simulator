#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 topLightingWorldPosition;
uniform vec4 frontLightingWorldPosition;

layout (location=0) in vec4 vertexCoordinates;
layout (location=1) in vec4 vertexNormals;
layout (location=2) in vec2 vertexTexturingCoordinates;
in float textureIndexIn;
flat out int textureIndex;

out vec2 interpolatedTexturingCoordinates;
out vec4 interpolatedColors;
out vec4 topLightingEyePosition;
out vec4 frontLightingEyePosition;
out vec4 v;
out vec4 n;

void main(void){
    gl_Position = P * V * M * vertexCoordinates;
    interpolatedTexturingCoordinates = vertexTexturingCoordinates;
    textureIndex = int(textureIndexIn);
    topLightingEyePosition = normalize(V * topLightingWorldPosition - V * M * vertexCoordinates);
    frontLightingEyePosition = normalize(V * frontLightingWorldPosition - V * M * vertexCoordinates);
    n = normalize(V * M * vertexNormals);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertexCoordinates);
}
