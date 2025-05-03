#version 330

in vec4 iC;
in vec4 l;
in vec4 n;
in vec4 v;
out vec4 pixelColor; //Output variable. Almost final pixel color.
void main(void) {
	vec4 _l=normalize(l);
	vec4 _n=normalize(n);
	vec4 _v=normalize(v);
	vec4 r=reflect(-_l,_n);
	float nl=clamp(dot(_l,_n),0,1);
	float rv=clamp(dot(r,_v),0,1);
    rv=pow(rv,25);
	pixelColor=iC*nl+rv;
}
