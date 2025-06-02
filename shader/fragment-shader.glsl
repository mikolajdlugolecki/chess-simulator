#version 330

uniform sampler2D whiteTileTexture;
uniform sampler2D blackTileTexture;
uniform sampler2D whiteFigureTexture;
uniform sampler2D blackFigureTexture;
uniform int whatToDraw;
uniform float ambientU;

in vec2 interpolatedTexturingCoordinates;
in vec4 topLightingEyePosition;
in vec4 frontLightingEyePosition;
in vec4 n;
in vec4 v;
flat in int textureIndex;

out vec4 pixelColor;

void main(void){
  	vec4 norm = normalize(n);
    vec4 viewDir = normalize(v);

    vec4 lightDir1 = normalize(topLightingEyePosition);
    vec4 reflectDir1 = reflect(-lightDir1, norm);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 50.0);

    vec4 lightDir2 = normalize(frontLightingEyePosition);
    vec4 reflectDir2 = reflect(-lightDir2, norm);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 50.0);

    vec3 color = vec3(1.0, 1.0, 1.0);

    vec3 ambient  = vec3(ambientU);
    vec3 diffuse  = diff1 * color * vec3(0.5) + diff2 * color * vec3(0.5);
    vec3 specular = spec1 * color * vec3(0.3) + spec2 * color * vec3(0.3);

    vec3 lighting = ambient + diffuse + specular;

    vec4 texColor;

    switch(whatToDraw){
        case 0:
            if(textureIndex == 0)
                texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
            else
                texColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
        break;
        case 1:
            texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
        break;
        case 2:
            texColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
        break;
        case 3:
            texColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
        break;
    }

    pixelColor = vec4(texColor.rgb * (ambient + diffuse) + specular, texColor.a);
}
