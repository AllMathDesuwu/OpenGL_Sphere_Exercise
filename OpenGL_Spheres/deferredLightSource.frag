#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec2 pass_xz;

uniform vec3 lightColor;
uniform float intensity;
uniform sampler2D diffuse0;
uniform vec3 camPos;	//not really used-- just here for compatibility (probably)

#define PI 3.141592653589793238462643383279
void main() {
	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	FragColor = vec4(texture(diffuse0, tc).rgb * lightColor * intensity, 1.0f);
	//FragColor = vec4(lightColor.rgb, 1.0f);
}