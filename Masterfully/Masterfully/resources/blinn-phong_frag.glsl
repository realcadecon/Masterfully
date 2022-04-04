#version 120

uniform vec3 col;

varying vec4 pos;
varying vec3 normal;


void main()
{
	vec3 ka = 0.2f * col / 255.f;
	vec3 kd = col / 255.f;
	vec3 ks = vec3(0.8, 0.7, 0.7);
	float s = 50.f;

	vec3 n = normalize(normal);
	// cd
	vec3 l_v = vec3(0, 1, 0) - vec3(pos);

	vec3 l_f = normalize(l_v);

	vec3 cd = kd*max(0, dot(l_f, n));

	// cs
	vec3 h = normalize(l_f - vec3(pos));

	vec3 cs = ks*pow(max(0, dot(h, n)), s);

	gl_FragColor = vec4(ka + cd + cs, 1.0);
}
