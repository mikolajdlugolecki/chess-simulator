#version 330

in vec2 i_tc;
out vec4 pixelColor;
uniform sampler2D edge;

void main(void){
	pixelColor = texture(edge, i_tc);
	
}
