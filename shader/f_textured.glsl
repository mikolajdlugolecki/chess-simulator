#version 330

in vec2 i_tc;
flat in int texIndex;
out vec4 pixelColor;
uniform sampler2D whiteTile;
uniform sampler2D blackTile;

void main(void){
	if(texIndex == 0)
		pixelColor = texture(blackTile, i_tc);
	else
		pixelColor = texture(whiteTile, i_tc);
}
