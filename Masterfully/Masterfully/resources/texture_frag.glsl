#version 120

uniform sampler2D texture0;

varying vec2 vTex; // don't use
varying vec3 normal;

void main()
{
	vec3 n = normalize(normal);
	gl_FragColor.rgb = texture2D(texture0, vTex).rgb + vec3(1, 0, 0);
	
	
}
