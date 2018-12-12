#version 130

in vec2 in_Position;
in vec3 in_Color;

uniform mat4 mvp;

out vec3 out_Color;
void main(void){
	gl_Position = mvp * vec4(in_Position, -1.0, 1.0);
	
	out_Color = in_Color;
}