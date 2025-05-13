#version 330

in float i_nl;
in vec2 i_tc;
out vec4 pixelColor;
uniform sampler2D tex;

void main(void){
    vec4 color = texture(tex, i_tc);
	pixelColor = vec4(color.rgb * i_nl, color.a);
}
