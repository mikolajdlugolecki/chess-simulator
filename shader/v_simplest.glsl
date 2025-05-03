#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 color;
in vec4 normal;
out vec4 l;
out vec4 n;
out vec4 v;
out vec4 iC;

void main(void) {
    //vec4 lp=vec4(0,0,-6,1); //world
    l=normalize(V*lp-V*M*vertex);  //eye
    n=normalize(V*M*normal);
    v=normalize(vec4(0,0,0,1)-V*M*vertex);
    iC=color;
    gl_Position=P*V*M*vertex;
}
