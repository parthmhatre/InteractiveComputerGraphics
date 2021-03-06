#version 130

in vec4 vPosition;
in vec4 vColor;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 LightPosition1;
uniform vec4 EyePosition;
out vec4 pass_color;
out vec4 fN;
out vec4 fE;
out vec4 fL;

void main()
{
	fN = vColor;
	fE = (EyePosition - vPosition).xyzw;
	fL = (LightPosition1 - vPosition).xyzw;		
	
	gl_Position = projection * model_view * vPosition; 
}