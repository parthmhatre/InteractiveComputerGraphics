#version 130

in vec4 fN;
in vec4 fL,fL1;
in vec4 fE; 
in vec3 newColor;
in vec2 valText;
uniform vec4 AmbientProduct1, DiffuseProduct1, SpecularProduct1;
uniform vec4 AmbientProduct2, DiffuseProduct2, SpecularProduct2;
uniform float Shininess;
uniform float flagValue;
uniform sampler2D texture;
out vec4 FragColor;

void main()
{
	// Normalize the input lighting vectors

	vec4 N = normalize(fN);
	vec4 E = normalize(fE);
	if (dot(N, E) < 0)  // the normal points away from the camera
      N = -N;
	vec4 L = normalize(fL);
	vec4 L1 = normalize(fL1);
	vec4 H = normalize( L + E );
	vec4 H1 = normalize( L1 + E );
	vec4 ambient1 = AmbientProduct1; 
	vec4 ambient2 = AmbientProduct2; 

	float dTerm1 = max(dot(L, N), 0.0);
	float dTerm2 = max(dot(L1, N), 0.0);
	vec4 diffuse1 = (dTerm1 * texture2D(texture,valText)).xyzw;
	vec4 diffuse2 = (dTerm2 * texture2D(texture,valText)).xyzw;

	float sTerm1 = pow(max(dot(N, H), 0.0), Shininess);
	float sTerm2 = pow(max(dot(N, H1), 0.0), Shininess);
	vec4 specular1 = sTerm1 * SpecularProduct1;
	vec4 specular2 = sTerm2 * SpecularProduct2;
	// discard the specular highlight if the light's behind the vertex
	if( dot(L, N) < 0.0 ){
		specular1 = vec4( 0.0, 0.0, 0.0, 0.0 );
		}
	if( dot(L1, N) < 0.0 ){
		specular2 = vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	if(flagValue == 0.0)
		FragColor = ambient1 + diffuse1 + specular1 + ambient2 + diffuse2 + specular2;
	else 
		FragColor = vec4(newColor, 1.0);
}
