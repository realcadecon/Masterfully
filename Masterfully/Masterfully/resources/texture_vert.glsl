#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor;
attribute vec2 aTex;

varying vec2 vTex;

void main()
{
	vTex = aTex;
	gl_Position = P * MV * aPos;
}
