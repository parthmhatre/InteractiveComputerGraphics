#version 130

in vec4 vPosition;
in vec3 vColor;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;
uniform vec4 EyePosition;
out vec4 fN;
out vec4 fE;
out vec4 fL,fL1;
out vec3 newColor;

void main()
{
	newColor=vColor;
	fN = vec4(vColor, 1.0);
	fE = (EyePosition - vPosition).xyzw;
	fL = (LightPosition1 - vPosition).xyzw;		
	fL1 =(LightPosition2 - vPosition).xyzw;
	gl_Position = projection * model_view * vPosition; 	
	gl_PointSize = 7.0;
}