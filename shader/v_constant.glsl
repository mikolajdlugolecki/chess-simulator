#version 330

layout (location=0) in vec4 vertex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main(void){
    gl_Position = P * V * M * vertex;
}
