#version 330

in vec2 interpolatedTexturingCoordinates;
flat in int textureIndex;
out vec4 pixelColor;
uniform sampler2D whiteTile;
uniform sampler2D blackTile;

void main(void){
	if(textureIndex == 0)
		pixelColor = texture(blackTile, interpolatedTexturingCoordinates);
	else
		pixelColor = texture(whiteTile, interpolatedTexturingCoordinates);
}
