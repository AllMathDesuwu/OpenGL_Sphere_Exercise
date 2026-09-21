#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D buff;
float exposure = 1.0;

void main() {
	vec3 color = texture(buff, TexCoords).rgb;

	vec3 mapped = vec3(1.0) - exp(-color * exposure);
	FragColor = vec4(mapped, 1.0f);

	//FragColor = vec4(0.22f, 0.22f, 0.60f, 1.0f);
}