#version 330 core
out vec4 FragColor;

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

//vec4 pointLight();
//vec4 directLight();
//vec4 spotLight();

void main()
{
	float ambient = 0.10f;

	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	vec3 lightDirection = normalize(lightPos - curPos);
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDir = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(diffuse0, tc); //vec4(color, 1.0f);
	//FragColor = texture(diffuse0, tc) * lightColor * (diffuse + ambient + specular);
}

/*
vec4 pointLight() {
	float ambient = 0.10f;

	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	vec3 lightDirection = normalize(lightPos - curPos);
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDir = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(diffuse0, tc) * lightColor * (diffuse + ambient + specular);
}

/*vec4 directLight() {
	float ambient = 0.10f;

	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDir = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(diffuse0, tc) * lightColor * (diffuse + ambient + specular);
}

vec4 spotLight() {
	float ambient = 0.10f;

	vec2 tc = texCoord;
	tc.x = (PI - atan(pass_xz.y, pass_xz.x)) / (2 * PI);

	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(Normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDir = reflect(-lightDirection, Normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(diffuse0, tc) * lightColor * (diffuse + ambient + specular);
}*/