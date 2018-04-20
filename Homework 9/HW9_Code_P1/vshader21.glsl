#version 130
in vec4 vPosition;
in vec3 vColor;
in vec2 buffer_texture;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;
uniform vec4 EyePosition;
out vec4 fN;
out vec4 fE;
out vec4 fL,fL1;
out vec3 newColor;
out vec2 valText;
void main()
{
	
	fN = vec4(vColor, 1.0);
	fE = (EyePosition - vPosition).xyzw;
	fL = (LightPosition1 - vPosition).xyzw;		
	fL1 =(LightPosition2 - vPosition).xyzw;
	gl_Position = projection * model_view * vPosition; 	
	valText=buffer_texture;
	newColor=vColor;
}