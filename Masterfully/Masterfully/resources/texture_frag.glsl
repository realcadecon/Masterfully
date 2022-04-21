#version 120
uniform sampler2D texture0;

varying vec2 vTex; // don't use

void main()
{
	gl_FragColor = vec4(texture2D(texture0, vec2(vTex)).rgb, 1.f);
	//gl_FragColor = vec4(1.f, 0.f, 0.f, 1.f);
}
