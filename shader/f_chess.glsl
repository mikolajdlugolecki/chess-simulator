#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
flat in int textureIndex;
out vec4 pixelColor;
uniform sampler2D whiteTile;
uniform sampler2D blackTile;
uniform sampler2D whiteFig;
uniform sampler2D blackFig;
uniform int useTexture;

void main(void){
	if(useTexture==1){
		if(textureIndex == 0)
			pixelColor = texture(blackTile, interpolatedTexturingCoordinates);
		else
			pixelColor = texture(whiteTile, interpolatedTexturingCoordinates);
	}
	else if(useTexture==0){
		pixelColor = texture(blackTile, interpolatedTexturingCoordinates);

	}
	else if(useTexture==2){
		pixelColor = texture(whiteFig, interpolatedTexturingCoordinates);
	}
	else if(useTexture==3){
		pixelColor = texture(blackFig, interpolatedTexturingCoordinates);
	}
}
