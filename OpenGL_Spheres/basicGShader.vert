#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 curPos;
out vec3 normal;

out vec2 pass_xz;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
	curPos = vec3(model * vec4(aPos, 1.0f)) / 10000000.0f;
	normal = normalMatrix * aNormal;
	vec3 correctedPos = curPos;
	gl_Position = camMatrix * vec4(correctedPos, 1.0f);

	gl_PointSize = 10.0f;

	pass_xz = vec2(aPos.x, aPos.z);
}