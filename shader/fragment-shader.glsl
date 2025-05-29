#version 330

in vec2 interpolatedTexturingCoordinates;
in vec4 interpolatedColors;
in vec4 l1, l2;
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
uniform float ambientU;

void main(void){
  	vec4 norm = normalize(n);
    vec4 viewDir = normalize(v);

    vec4 lightDir1 = normalize(l1);
    vec4 reflectDir1 = reflect(-lightDir1, norm);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 100.0);

    vec4 lightDir2 = normalize(l2);
    vec4 reflectDir2 = reflect(-lightDir2, norm);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 100.0);

    vec3 ambient  = vec3(ambientU);
    vec3 diffuse  = (diff1 + diff2) * vec3(0.5);
    vec3 specular = (spec1 + spec2) * vec3(0.3);

    vec3 lighting = ambient + diffuse + specular;

    vec4 texColor;

    if(whatToDraw == 0){
		if(textureIndex == 0)
        	texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
		else
			texColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
    }
    else if(whatToDraw == 1){
        texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
    }
    else if(whatToDraw == 2){
        texColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
    }
    else if(whatToDraw == 3){
        texColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
    }
    pixelColor = vec4(texColor.rgb * lighting, texColor.a);
}
