#include "Quad.h"
#include <iostream>

const Quad_Vertex Quad::quadVertices[] = {
		Quad_Vertex{glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
		Quad_Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		Quad_Vertex{glm::vec3(1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
		Quad_Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

Quad::Quad() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Quad_Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Quad_Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind to avoid accidents
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//YOU ARE RESPONSIBLE FOR BINDING YOUR OWN TEXTURE BEFOREHAND!!!
void Quad::Draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}