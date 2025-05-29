#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
in vec4 l;
in vec4 n;
in vec4 v;
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
	vec4 _l = normalize(l);
	vec4 _n = normalize(n);
	vec4 _v = normalize(v);
	vec4 r = reflect(-_l, _n);
	float nl = clamp(dot(_l, _n), 0, 1);
	float rv = clamp(dot(r, _v), 0, 1);
	vec4 texColor;
    rv = pow(rv, 50);
	if(whatToDraw == 0){
		if(textureIndex == 0)
			texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
		else
			texColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
	}
	else if(whatToDraw == 1)
		texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
	else if(whatToDraw == 2)
		texColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
	else if(whatToDraw == 3)
		texColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
	pixelColor = vec4(texColor.rgb * nl + rv, texColor.a);
}
