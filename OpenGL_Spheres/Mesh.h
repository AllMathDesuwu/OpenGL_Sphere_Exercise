#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include <vector>
#include <list>
#include "Texture.h"

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO VAO;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	//Mesh(std::vector<Texture>& textures);

	void Draw(Shader& shader, Camera& camera);
	void Delete();

protected:
	virtual void updateModel(Shader& shader);
};


#endif