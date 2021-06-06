#version 150

in vec4	ciPosition;

out vec3 vPosition;

uniform mat4 ciModelViewProjection;

void main(void)
{
	vPosition = vec3(ciPosition);
	gl_Position = ciModelViewProjection * ciPosition;
}
