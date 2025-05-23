#version 330

in vec2 i_tc;
uniform sampler2D tex;
out vec4 pixelColor;

void main(void){
	pixelColor = texture(tex, i_tc);
}
