#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
flat in int textureIndex;
out vec4 pixelColor;
uniform sampler2D whiteTile;
uniform sampler2D blackTile;
uniform bool useTexture;

void main(void){
	if(useTexture){
		if(textureIndex == 0)
			pixelColor = texture(blackTile, interpolatedTexturingCoordinates);
		else
			pixelColor = texture(whiteTile, interpolatedTexturingCoordinates);
	}
	else
		pixelColor = interpolatedColors;
}
