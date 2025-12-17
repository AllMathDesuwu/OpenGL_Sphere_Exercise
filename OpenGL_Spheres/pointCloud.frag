#version 330 core
out vec4 FragColor;

in vec3 pos;

void main()
{
	FragColor = vec4(0.5f + 0.5f * pos.y, 0.0f, 0.5f + 0.5f * pos.y, 1.0f);
}