#include "Sphere.h"

Sphere::Sphere(int numSectors, int numStacks, float radius, std::vector<Texture>& textures, float mass, std::list<SimObject*>& world, bool addLight) : SimObject((std::vector<Vertex>&)Sphere::generateUVSphereVert(numSectors, numStacks, radius), (std::vector<GLuint>&)Sphere::generateUVSphereIdx(numSectors, numStacks), textures, mass, (std::list<SimObject*>&)world, addLight){
	//TODO: consider mallocing these vectors
	Sphere::radius = radius;

	/*int i = 0;
	int j = 0;
	int k = 0;
	for (GLuint idx : indices) {
		std::cout << vertices[idx].texUV.x << " " << vertices[idx].texUV.y << "/";
		i++;
		if (i >= 3) {
			i = 0;
			j++;
			std::cout << std::endl;
		}
		if (k == 0 && j == numSectors) {
			j = 0;
			k++;
			std::cout << "==========" << std::endl;
		}
		else if (j == 2 * numSectors) {
			j = 0;
			k++;
			std::cout << "==========" << std::endl;
		}
	}*/
}

void Sphere::generateUVSphere(int numSectors, int numStacks, float radius, std::vector<Vertex>& vertOut, std::vector<GLuint>& indxOut) {
	const float sectorNormalized = 1.0f / numSectors;
	const float stackNormalized = 1.0f / numStacks;

	int  index = 0;	//index counts position in array (divide by three gives vertex), subindex gives x, y, or z
	for (int i = 0; i <= numStacks; i++) {	//number of vertical units
		float phi = (90 - 180 * (i / (float)numStacks)) * (pi / 180);
		float y = radius * sinf(phi);
		float adjustedRadius = radius * cosf(phi);
		for (int j = 0; j < numSectors; j++) {
			float theta = (360 * (j / (float)numSectors)) * (pi / 180);
			float x = adjustedRadius * cosf(theta);
			float z = adjustedRadius * sinf(theta);

			glm::vec3 sphereVert = glm::vec3(x, y, z);
			vertOut.emplace_back(Vertex{ sphereVert, sphereVert / radius, glm::vec3(0.20f, 0.20f, 0.66f), glm::vec2(j * sectorNormalized, i * stackNormalized) });
			
			index++;

			if (i == 0 || i == numStacks) break;
		}
	}

	index = 0;	//represents the index of the current float in the array
	int vertexIdx = 0;	//represents the index of the current vertex
	const int NEXT_STACK = numSectors;	//only works in general case--not when cur layer or next layer doesn't have proper number of vertices
	for (int i = 0; i < numStacks; i++) {
		int firstIdxInStack = index;
		int offset = 0;
		int vertexOffset = 0;
		if (!(i == 0 || i == numStacks - 1)) {
			for (int j = 0; j < numSectors; j++) {
				//upper triangle
				int tempIdx = index + offset;
				//indxOut[tempIdx] = vertexIdx + vertexOffset;
				//indxOut[tempIdx + 1] = vertexIdx + NEXT_STACK + vertexOffset;
				//indxOut[tempIdx + 2] = vertexIdx + ((vertexOffset + 1) % numSectors); //modulo in case it wraps around

				////lower triangle
				//indxOut[tempIdx + 3] = vertexIdx + ((vertexOffset + 1) % numSectors);
				//indxOut[tempIdx + 4] = vertexIdx + NEXT_STACK + vertexOffset;
				//indxOut[tempIdx + 5] = vertexIdx + NEXT_STACK + ((vertexOffset + 1) % numSectors);

				indxOut.emplace_back(vertexIdx + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK + vertexOffset);
				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));

				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));
				indxOut.emplace_back(vertexIdx + NEXT_STACK + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK + ((vertexOffset + 1) % numSectors));

				offset += 6;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
		else if (i == 0) {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				/*indxOut[tempIdx] = vertexIdx;
				indxOut[tempIdx + 1] = vertexIdx + 1 + (vertexOffset % numSectors);
				indxOut[tempIdx + 2] = vertexIdx + 1 + ((vertexOffset + 1) % numSectors);*/

				indxOut.emplace_back(vertexIdx);
				indxOut.emplace_back(vertexIdx + 1 + (vertexOffset % numSectors));
				indxOut.emplace_back(vertexIdx + 1 + ((vertexOffset + 1) % numSectors));

				offset += 3;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx++;
		}
		else {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				//indxOut[tempIdx] = vertexIdx + vertexOffset;
				//indxOut[tempIdx + 1] = vertexIdx + NEXT_STACK;	//DON'T ADD VERTEX OFFSET because all of these triangles will share this vertex-- the "Southern"-most vertex regardless of offset from the first index of the stack
				//indxOut[tempIdx + 2] = vertexIdx + ((vertexOffset + 1) % numSectors);

				indxOut.emplace_back(vertexIdx + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK);
				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));

				offset += 3;
				vertexOffset++;
				//TODO: FIX THIS!!! Remember, you want the index of the VERTEX, not the index of an individual element fo a vertex.
			}
			//keeping track of these at this point probably doesn't matter much
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
	}
}

std::vector<Vertex> Sphere::generateUVSphereVert(int numSectors, int numStacks, float radius) {
	const float sectorNormalized = 1.0f / numSectors;
	const float stackNormalized = 1.0f / numStacks;

	std::vector<Vertex> vertOut;

	int  index = 0;	//index counts position in array (divide by three gives vertex), subindex gives x, y, or z
	for (int i = 0; i <= numStacks; i++) {	//number of vertical units
		float phi = (90 - 180 * (i / (float)numStacks)) * (pi / 180);
		float y = radius * sinf(phi);
		float adjustedRadius = radius * cosf(phi);
		for (int j = 0; j < numSectors; j++) {
			float theta = (360 * (j / (float)numSectors)) * (pi / 180);
			float x = adjustedRadius * cosf(theta);
			float z = adjustedRadius * sinf(theta);

			glm::vec3 sphereVert = glm::vec3(x, y, z);
			vertOut.emplace_back(Vertex{ sphereVert, sphereVert / radius, glm::vec3(0.20f, 0.20f, 0.66f), glm::vec2((numSectors - j + 0.50f) * sectorNormalized, (numStacks - i) * stackNormalized) });
			//std::cout << vertOut.back().texUV.x << " " << vertOut.back().texUV.y << std::endl;

			index++;

			if (i == 0 || i == numStacks) break;
		}
		//std::cout << std::endl;
	}

	return vertOut;
}

std::vector<GLuint> Sphere::generateUVSphereIdx(int numSectors, int numStacks) {
	std::vector<GLuint> indxOut;

	int index = 0;	//represents the index of the current float in the array
	int vertexIdx = 0;	//represents the index of the current vertex
	const int NEXT_STACK = numSectors;	//only works in general case--not when cur layer or next layer doesn't have proper number of vertices
	for (int i = 0; i < numStacks; i++) {
		int firstIdxInStack = index;
		int offset = 0;
		int vertexOffset = 0;
		if (!(i == 0 || i == numStacks - 1)) {
			for (int j = 0; j < numSectors; j++) {
				//upper triangle
				int tempIdx = index + offset;
				indxOut.emplace_back(vertexIdx + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK + vertexOffset);
				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));

				//lower triangle
				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));
				indxOut.emplace_back(vertexIdx + NEXT_STACK + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK + ((vertexOffset + 1) % numSectors));

				offset += 6;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
		else if (i == 0) {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				indxOut.emplace_back(vertexIdx);
				indxOut.emplace_back(vertexIdx + 1 + (vertexOffset % numSectors));
				indxOut.emplace_back(vertexIdx + 1 + ((vertexOffset + 1) % numSectors));

				offset += 3;
				vertexOffset++;
			}
			index = firstIdxInStack + offset;
			vertexIdx++;
		}
		else {
			for (int j = 0; j < numSectors; j++) {
				int tempIdx = index + offset;
				indxOut.emplace_back(vertexIdx + vertexOffset);
				indxOut.emplace_back(vertexIdx + NEXT_STACK);
				indxOut.emplace_back(vertexIdx + ((vertexOffset + 1) % numSectors));

				offset += 3;
				vertexOffset++;
			}
			//keeping track of these at this point probably doesn't matter much
			index = firstIdxInStack + offset;
			vertexIdx += vertexOffset;
		}
	}

	return indxOut;
}