#version 120

varying vec3 vNor;
uniform vec3 col;

void main()
{
	//vec3 normal = normalize(vNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	//vec3 color = 0.5*normal + 0.5;
	gl_FragColor = vec4(col/255, 1.0);
}
