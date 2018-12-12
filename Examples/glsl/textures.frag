#version 130

in vec3 out_Color;
in vec2 out_UV;

uniform sampler2D mySampler;

out vec4 gl_FragColor;
void main(void){
	gl_FragColor = vec4(out_Color, 1.0) * texture2D(mySampler, out_UV);
}