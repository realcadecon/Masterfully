#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 VM;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 pos;
varying vec3 normal;


void main()
{
	pos = MV * aPos;
	vec4 tempNor = (VM * vec4(aNor, 0.0));
	normal = vec3(tempNor);
	gl_Position = P * (MV * aPos);
}
