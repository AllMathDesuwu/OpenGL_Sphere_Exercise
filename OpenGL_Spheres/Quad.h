#ifndef QUAD_CLASS_H
#define QUAD_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Quad_Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
};

class Quad {
public:
	GLuint VBO;
	GLuint VAO;
	const static Quad_Vertex quadVertices[];

	Quad();
	void Draw();
};

#endif