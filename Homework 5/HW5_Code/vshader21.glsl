#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 pass_color;
uniform mat4 model_view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * model_view * vPosition;
	pass_color = vColor ;
}