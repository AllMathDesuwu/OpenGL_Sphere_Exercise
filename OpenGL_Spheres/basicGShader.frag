#version 330 core
out vec4 FragColor;

in vec3 curPos;
in vec3 normal;

in vec2 pass_xz;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

#define PI 3.141592653589793238462643383279
float exposure = 1.0;

void main()
{	
	vec3 color = vec3(normal.xyz);

	vec3 mapped = vec3(1.0) - exp(-color * exposure);
	FragColor = vec4(mapped, 1.0f);
}