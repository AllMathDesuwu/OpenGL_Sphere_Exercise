#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

out vec2 pass_xz;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
	vec4 rawPos = (model * vec4(aPos, 1.0f));
	vec4 correctedPos = vec4(rawPos.xyz / 10000000.0f, rawPos.w);
	gl_Position = camMatrix * correctedPos;

	color = aColor;
	texCoord = aTex;

	pass_xz = vec2(aPos.x, aPos.z);
}
