#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
flat in int textureIndex;
out vec4 pixelColor;
uniform sampler2D whiteTileTexture;
uniform sampler2D blackTileTexture;
uniform sampler2D whiteFigureTexture;
uniform sampler2D blackFigureTexture;
uniform bool drawChessboard;
uniform bool drawChessboardBase;
uniform bool drawWhiteFigure;
uniform bool drawBlackFigure;

void main(void){
	if(drawChessboard){
		if(textureIndex == 0)
			pixelColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
		else
			pixelColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
	}
	else if(drawChessboardBase)
		pixelColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
	else if(drawWhiteFigure)
		pixelColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
	else if(drawBlackFigure)
		pixelColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
}
