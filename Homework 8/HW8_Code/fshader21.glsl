#version 130

in vec4 pass_color;
in vec4 fN;
in vec4 fL,fL1;
in vec4 fE; 
out vec4 FragColor;
uniform vec4 AmbientProduct1, DiffuseProduct1, SpecularProduct1;
uniform float Shininess;
uniform vec4 sendColor;
uniform int onclickColor;

void main()
{
	if(onclickColor == 0) {
		// Normalize the input lighting vectors

		vec4 N = normalize(fN);
		vec4 E = normalize(fE);
		vec4 L = normalize(fL);
		vec4 H = normalize( L + E );
		vec4 ambient1 = AmbientProduct1; 

		float dTerm1 = max(dot(L, N), 0.0);
		vec4 diffuse1 = (dTerm1 * DiffuseProduct1).xyzw;

		float sTerm1 = pow(max(dot(N, H), 0.0), Shininess);
		vec4 specular1 = sTerm1 * SpecularProduct1;
		// discard the specular highlight if the light's behind the vertex
		if( dot(L, N) < 0.0 ){
			specular1 = vec4( 0.0, 0.0, 0.0, 0.0 );
		}
		FragColor = ambient1 + diffuse1 + specular1;
	}
	else
		FragColor = sendColor;
}
