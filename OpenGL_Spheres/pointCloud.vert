#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * vec4(aPos, 1.0f);
	gl_PointSize = 10.0f;
	pos = aPos;
}