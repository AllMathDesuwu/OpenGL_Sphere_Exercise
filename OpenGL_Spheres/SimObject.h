#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include "Mesh.h"

struct LightParams {
	float intensity;
	float Linear;
	float Quadratic;
	glm::vec3 Color;
};

const float GRAV_CONSTANT = 6.6743f * ((float)std::pow(10, -11)); //should be 10, -11 but uh... gravity was too strong

class SimObject : public Mesh {
public:
	//it might be a good idea in the future to distinguish between absolute transforms and relative ones in the future
	//relative transforms might be done via methods which perform rotations and the like (and generally shift coordinate system around) while absolute transforms directly change values
	glm::vec3 Position;
	glm::vec3 PrevPosition;	//really should have copies of everything but for our purposes just Position should be fine
	glm::vec3 Velocity;
	glm::vec3 Acceleration;
	glm::vec3 Orientation;	//should be thought of as rotation of points from their default positions along each of the three axes
	glm::vec3 Omega;	//angular velocity
	//yeah, if you want torque, have fun with that
	glm::vec3 SumOfForces;
	LightParams light;

	float mass;
	std::list<SimObject*>* world;
	std::list<SimObject*>::iterator worldListPos;

	static std::list<SimObject*> lights;


	SimObject(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, float mass, std::list<SimObject*>* world, bool addLight = false);
	//SimObject(std::vector<Texture>& textures, float mass, std::list<SimObject>& world);
	//~SimObject();

	void InitialConditions();
	void InitialConditions(glm::vec3 Position, glm::vec3 Velocity, glm::vec3 Acceleration, glm::vec3 Orientation, glm::vec3 Omega);
	void update(float deltaTime);
	void SetLightParams(float intensity, float Linear, float Quadratic, glm::vec3 Color);
	void SetLightParams(float Linear, float Quadratic, glm::vec3 Color);
	static void updatePrevPosition(std::list<SimObject*>* world);
	glm::vec3 calcAndSetForces();
	glm::vec3 calcForces(glm::vec3 offset);
protected:
	void updateModel(Shader& shader) override;
};


#endif