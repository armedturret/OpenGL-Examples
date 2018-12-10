#version 130

in vec3 out_Color;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor = vec4(out_Color, 1.0);
}