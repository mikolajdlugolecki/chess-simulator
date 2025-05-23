#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
flat in int textureIndex;
out vec4 pixelColor;
uniform sampler2D whiteTileTexture;
uniform sampler2D blackTileTexture;
uniform sampler2D whiteFigureTexture;
uniform sampler2D blackFigureTexture;
uniform int whatToDraw;
uniform bool drawChessboard;
uniform bool drawChessboardBase;
uniform bool drawWhiteFigure;
uniform bool drawBlackFigure;

void main(void){
	if(whatToDraw == 0){
		if(textureIndex == 0)
			pixelColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
		else
			pixelColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
	}
	else if(whatToDraw == 1)
		pixelColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
	else if(whatToDraw == 2)
		pixelColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
	else if(whatToDraw == 3)
		pixelColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
}
