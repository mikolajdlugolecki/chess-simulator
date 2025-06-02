#version 330

uniform sampler2D whiteTileTexture;
uniform sampler2D blackTileTexture;
uniform sampler2D whiteFigureTexture;
uniform sampler2D blackFigureTexture;
uniform int whatToDraw;

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

    vec3 ambient  = vec3(0.2);
    vec3 diffuse  = (diff1 + diff2) * vec3(0.5); 
    vec3 specular = vec3(spec1 + spec2);

    vec4 texColor;

    switch(whatToDraw){
        case 0:
            if(textureIndex == 0)
                texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
            else
                texColor = texture(whiteTileTexture, interpolatedTexturingCoordinates);
            specular *= vec3(0.3);
        break;
        case 1:
            texColor = texture(blackTileTexture, interpolatedTexturingCoordinates);
            specular *= vec3(0.3);
        break;
        case 2:
            texColor = texture(whiteFigureTexture, interpolatedTexturingCoordinates);
            specular *= vec3(0.9);
        break;
        case 3:
            texColor = texture(blackFigureTexture, interpolatedTexturingCoordinates);
            specular *= vec3(0.9);
        break;
    }

    pixelColor = vec4(texColor.rgb * (ambient + diffuse) + specular, texColor.a);
}
