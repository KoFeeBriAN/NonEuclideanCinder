#version 150

in vec3	vPosition;

out vec4 	fColor;

uniform samplerCube uCubeMapTex;

void main( void )
{
	fColor = texture( uCubeMapTex, vPosition );
}