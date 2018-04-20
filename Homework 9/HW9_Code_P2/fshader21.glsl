#version 130

in vec4 fN;
in vec4 fL,fL1;
in vec4 fE;
in vec4 temp_vPosition; 
vec4 newTemp;
out vec4 FragColor;
uniform vec4 AmbientProduct1, DiffuseProduct1, SpecularProduct1;
uniform vec4 AmbientProduct2, DiffuseProduct2, SpecularProduct2;
uniform float Shininess;

vec3 BrickColor = vec3(0.0,0.0,0.5);
vec3 MortarColor=vec3(0.2,0.2,0.2);
vec2  BrickSize=vec2(-0.2,0.1);
vec2  BrickPct=vec2(0.8,0.8);

void main()
{

		// Normalize the input lighting vectors

		vec4 N = normalize(fN);
		vec4 E = normalize(fE);
		vec4 L = normalize(fL);
		vec4 L1 = normalize(fL1);
		vec4 H = normalize( L + E );
		vec4 H1 = normalize( L1 + E );
		vec4 ambient1 = AmbientProduct1;
		vec4 ambient2 = AmbientProduct2;

		vec3  color;
    vec2  position;
	vec2 useBrick;
 
    position = vec2(temp_vPosition) / BrickSize;

    if (fract(position.y * 0.5) > 0.5)
        position.x += 0.5;

    position = fract(position);

    useBrick = step(position, BrickPct);

    color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y); 
	newTemp = vec4(color.x,color.y,color.z,1.0);

		float dTerm1 = max(dot(L, N), 0.0);
		float dTerm2 = max(dot(L1, N), 0.0);
		vec4 diffuse1 = (dTerm1 * newTemp).xyzw;
		vec4 diffuse2 = (dTerm2 * newTemp).xyzw;
		
		float sTerm1 = pow(max(dot(N, H), 0.0), Shininess);
		float sTerm2 = pow(max(dot(N, H1), 0.0), Shininess);
		vec4 specular1 = sTerm1 * SpecularProduct1;
		vec4 specular2 = sTerm2 * SpecularProduct2;
		// discard the specular highlight if the light's behind the vertex
		if( dot(L, N) < 0.0 ){
			specular1 = vec4( 0.0, 0.0, 0.0, 1.0 );
		}
		if( dot(L1, N) < 0.0 ){
			specular2 = vec4( 0.0, 0.0, 0.0, 1.0 );
		}
		FragColor = ambient1 + diffuse1 + specular1 + ambient2 + diffuse2 + specular2;
}
