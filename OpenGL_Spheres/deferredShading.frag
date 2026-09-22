#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
};
uniform int numLights;
const int MAX_LIGHTS = 1;	//max number of lights in a single iteration
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

const float AMBIENT = 0.0005f;
const float GAMMA = 2.2f;
void main() {
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

	vec3 lighting = Diffuse * AMBIENT;
	vec3 viewDir = normalize(viewPos - FragPos);

	float outVar;
	for(int i = 0; i < numLights; i++) {
		vec3 lightDir = normalize(lights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0f) * Diffuse * lights[i].Color;

		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
		vec3 specular = lights[i].Color * spec * Specular;

		float distance = length(lights[i].Position - FragPos);
		float attenuation = 1.0 / (1.0 + distance * (lights[i].Linear + lights[i].Quadratic * distance));

		outVar = 1.0 + (distance * (lights[i].Linear + lights[i].Quadratic * distance));
		diffuse *= attenuation;
		specular *= attenuation;

		lighting += diffuse + specular;
	}

	//vec3 lightDir = normalize(lights[0].Position - FragPos);
	lighting = pow(lighting, vec3(1.0 / GAMMA));	//gamma correction
	FragColor = vec4(lighting, 1.0);
	//FragColor.rgb = outAtten * max(dot(Normal, lightDir), 0.0f) * vec3(1.0f, 1.0f, 1.0f);
	//.a = 1.0f;
	//FragColor = texture(gAlbedoSpec, TexCoords);
}