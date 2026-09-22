#include "NumericMethods.h"

//TODO: Please see if Yuanda can explain how to do this with generics because at the moment this is kinda atrocious...

glm::vec3 euler(SimObject& object, float deltaTime) {
	float scaledTime = deltaTime * TIME_SCALE;

	for (int i = 0; i < 6; i++) {
		object.calcAndSetForces();
		object.Velocity += (1.0f / 6) * object.Acceleration * scaledTime;
		object.Position += (1.0f / 6) * object.Velocity * scaledTime;
	}
	return object.Position;
}

glm::vec3 eulerMid(SimObject& object, float deltaTime) {
	float scaledTime = deltaTime * TIME_SCALE;

	//first iteration
	object.calcAndSetForces();
	glm::vec3 kV1 = object.Acceleration * scaledTime;
	glm::vec3 kX1 = object.Velocity * scaledTime;

	//second iteration
	glm::vec3 accel = object.calcForces(0.5f * kX1) / object.mass;	//might want to modify this to possibly consider acceleration when finding "midpoint"?
	glm::vec3 kV2 = accel * scaledTime;
	glm::vec3 kX2 = (object.Velocity + (0.5f * kV1)) * scaledTime;

	//bookkeeping
	object.Velocity += kV2;
	object.Position += kX2;
	return object.Position;
}

//perhaps we can see why this could start to get expensive...
glm::vec3 rk4(SimObject& object, float deltaTime) {
	float scaledTime = deltaTime * TIME_SCALE;

	//first iteration
	glm::vec3 accel = object.calcAndSetForces() / object.mass;
	glm::vec3 kV1 = accel * scaledTime;
	glm::vec3 kX1 = object.Velocity * scaledTime;

	//second iteration
	accel = object.calcForces(0.5f * kX1) / object.mass;
	glm::vec3 kV2 = accel * scaledTime;
	glm::vec3 kX2 = (object.Velocity + (0.5f * kV1)) * scaledTime;

	//third iteration
	accel = object.calcForces(0.5f * kX2) / object.mass;
	glm::vec3 kV3 = accel * scaledTime;
	glm::vec3 kX3 = (object.Velocity + (0.5f * kV2)) * scaledTime;

	//fourth iteration
	accel = object.calcForces(kX3) / object.mass;
	glm::vec3 kV4 = accel * scaledTime;
	glm::vec3 kX4 = (object.Velocity + (kV3)) * scaledTime;

	//bookkeeping
	object.Velocity += (1.0f / 6) * (kV1 + 2.0f * kV2 + 2.0f * kV3 + kV4);
	object.Position += (1.0f / 6) * (kX1 + 2.0f * kX2 + 2.0f * kX3 + kX4);
	return object.Position;
}

//template<typename ReturnType, typename ...OtherTypes>
//numericResult_t<ReturnType, 1> euler(std::function<ReturnType(OtherTypes...)> func) {
//	numericResult_t<ReturnType, 1> result;
//
//	result[0] = 
//}