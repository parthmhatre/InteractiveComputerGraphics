#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 fN;
out vec4 fE;
out vec4 fL,fL1;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 AmbientProduct1, DiffuseProduct1, SpecularProduct1;
uniform vec4 AmbientProduct2, DiffuseProduct2, SpecularProduct2;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;
uniform float Shininess;
uniform vec4 EyePosition;

void main()
{
	fN = vColor;
	fE = (EyePosition - vPosition).xyzw;
	fL = LightPosition1;		
	fL1 = LightPosition2;
	
	gl_Position = projection * model_view * vPosition; 	
}