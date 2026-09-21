#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 curPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

in vec2 pass_xz;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

#define PI 3.141592653589793238462643383279

void main()
{	
	float specularLight = 1.0f;

	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	gPosition = curPos;

	gNormal = normalize(Normal);

	gAlbedoSpec = vec4(texture(diffuse0, tc).rgb, specularLight);
}