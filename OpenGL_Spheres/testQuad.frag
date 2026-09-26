#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D buff;

void main() {
	FragColor = vec4(texture(buff, TexCoords).rgb, 1.0f);
}