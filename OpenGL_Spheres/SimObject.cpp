#include "SimObject.h"
#include "NumericMethods.h"
#include <cmath>

std::list<SimObject*> SimObject::lights;

SimObject::SimObject(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, float mass, std::list<SimObject*>& world, bool addLight) : Mesh(vertices, indices, textures) {
	SimObject::mass = mass;
	SimObject::world = world;
	world.push_back(this);
	SimObject::worldListPos = std::prev(world.end(), 1);

	if (addLight) lights.push_back(this);

	InitialConditions();
}

//SimObject::SimObject(std::vector<Texture>& textures, float mass, std::list<SimObject>& world) : Mesh(textures) {
//	SimObject::mass = mass;
//	SimObject::world = world;
//	world.push_back((*this));
//	SimObject::worldListPos = std::prev(world.end(), 1);
//
//	InitialConditions();
//}

//SimObject::~SimObject() {
//	std::cout << world.size() << std::endl;
//	world.erase(worldListPos);	//remove yourself from world list
//}

void SimObject::InitialConditions(glm::vec3 Position, glm::vec3 Velocity, glm::vec3 Acceleration, glm::vec3 Orientation, glm::vec3 Omega) {
	SimObject::Position = Position;
	SimObject::Velocity = Velocity;
	SimObject::Acceleration = Acceleration;
	SimObject::Orientation = Orientation;
	SimObject::Omega = Omega;
	SumOfForces = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SimObject::InitialConditions() {
	InitialConditions(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
}

//note to self: in the future, maintain a data structure that keeps a record of all pair-wise distances between objects-- and update it whenever an object's position gets updated
glm::vec3 SimObject::calcAndSetForces() {
	glm::vec3 forces = glm::vec3(0.0f, 0.0f, 0.0f);
	for (SimObject* object : world) {
		if (this == object) continue;
		glm::vec3 dir = object->Position - this->Position;
		double sqrMag = (glm::dot(dir, dir));
		dir = dir / std::sqrtf(sqrMag);
		//std::cout << glm::dot(dir, dir) << std::endl;
		//std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
		//std::cout << object->Position.x << " " << object->Position.y << " " << object->Position.z << std::endl;
		forces += (float)(GRAV_CONSTANT * object->mass * this->mass / sqrMag) * dir;
	}
	
	this->SumOfForces = forces;
	this->Acceleration = forces / mass;
	return forces;
}

glm::vec3 SimObject::calcForces(glm::vec3 offset) {
	glm::vec3 forces = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 adjustedPos = this->Position + offset;
	for (SimObject* object : world) {
		if (this == object) continue;
		glm::vec3 dir = object->Position - this->Position;
		double sqrMag = (glm::dot(dir, dir));
		dir = dir / std::sqrtf(sqrMag);
		//std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
		//std::cout << object->Position.x << " " << object->Position.y << " " << object->Position.z << std::endl;
		forces += (float)(GRAV_CONSTANT * object->mass * this->mass / ((float)sqrMag)) * dir;
	}

	return forces;
}

void SimObject::update(float deltaTime) {
	euler(*this, deltaTime);
}

void SimObject::updateModel(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	//chance for gimbal lock...
	//also putting some ifs since for our case 99% of the time only the y component will be used
	if (Orientation.x != 0) model = glm::rotate(model, glm::radians(Orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	if (Orientation.y != 0) model = glm::rotate(model, glm::radians(Orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	if (Orientation.z != 0) model = glm::rotate(model, glm::radians(Orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, Position);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 normalMat = glm::mat3(1.0f);
	normalMat = glm::transpose(glm::inverse(model));	//don't ask me why this works :P
	glUniformMatrix3fv(glGetUniformLocation(shader.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMat));
}

void SimObject::SetLightParams(float Linear, float Quadratic, glm::vec3 Color) {
	this->light.Linear = Linear;
	this->light.Quadratic = Linear;
	this->light.Color = Color;
}