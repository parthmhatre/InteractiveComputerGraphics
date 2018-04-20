#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 pass_color;
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
uniform float gouraud_phong;

void main()
{
	if ( gouraud_phong == 0){
		// Transform vertex position into eye coordinates
		vec4 pos = (model_view * vPosition).xyzw;
		// Light defined in camera frame
		vec4 L1 = normalize( LightPosition1 - pos );
		vec4 L2 = normalize( LightPosition2 - pos );
		vec4 E = normalize( -pos );
		vec4 H1 = normalize( L1 + E );
		vec4 H2 = normalize( L2 + E );
		// Transform vertex normal into eye coordinates
		vec4 N = normalize( model_view * vColor).xyzw;

		// Compute terms in the illumination equation
		vec4 ambient1 = AmbientProduct1.xyzw;
		vec4 ambient2 = AmbientProduct2.xyzw;
		float dTerm1 = max( dot(L1, N), 0.0 );
		float dTerm2 = max( dot(L2, N), 0.0 );
		vec4 diffuse1 = (dTerm1*DiffuseProduct1).xyzw;
		vec4 diffuse2 = (dTerm2*DiffuseProduct2).xyzw;
		float sTerm1 = pow( max(dot(N, H1), 0.0), Shininess );
		float sTerm2 = pow( max(dot(N, H2), 0.0), Shininess );
		vec4 specular1 = (sTerm1 * SpecularProduct1).xyzw;
		vec4 specular2 = (sTerm2 * SpecularProduct2).xyzw;
		
		if( dot(L1, N) < 0.0 ) 
			specular1 = vec4(0.0, 0.0, 0.0, 0.0);

		if( dot(L2, N) < 0.0 ) 
			specular2 = vec4(0.0, 0.0, 0.0, 0.0);
		gl_Position = projection * model_view * vPosition;
		pass_color =  ambient1 + diffuse1 + specular1 + ambient2 + diffuse2 + specular2;
	}
	else if( gouraud_phong == 1){
		fN = vColor;
		fE = (EyePosition - vPosition).xyzw;
		fL = LightPosition1;		
 		fL1 = LightPosition2;
		
		gl_Position = projection * model_view * vPosition; 
	}
}