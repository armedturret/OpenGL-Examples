#version 130

in vec2 in_Position;
in vec3 in_Color;

out vec3 out_Color;
void main(void){
	gl_Position = vec4(in_Position, 0.0, 1.0);
	
	out_Color = in_Color;
}