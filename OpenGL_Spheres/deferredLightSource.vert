#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec3 curPos = aPos / 10000000.0f;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
