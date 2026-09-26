#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "SimObject.h"

const double pi = 2 * acos(0.0);

class Sphere : public SimObject {
public:
	float radius;

	Sphere(int numSectors, int numStacks, float radius, std::vector<Texture>& textures, float mass, std::list<SimObject*>* world, bool addLight = false);

	static void generateUVSphere(int numSectors, int numStacks, float radius, std::vector<Vertex>& vertOut, std::vector<GLuint>& indxOut);
	static std::vector<Vertex> generateUVSphereVert(int numSectors, int numStacks, float radius);
	static std::vector<GLuint> generateUVSphereIdx(int numSectors, int numStacks);
};

#endif