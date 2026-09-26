#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloom;

void main(){
	vec3 sceneColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloom, TexCoords).rgb;

	FragColor = vec4(sceneColor + bloomColor, 1.0f);
}