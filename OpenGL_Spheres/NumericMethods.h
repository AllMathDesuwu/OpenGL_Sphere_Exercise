#ifndef NUMERIC_METHODS_H
#define NUMERIC_METHODS_H

#include <iostream>
#include <functional>

#include "SimObject.h"
#include <glm/glm.hpp>

const float TIME_SCALE = 3600.0f * 24 * 7 * 0.125;

//TODO: Ask Yuanda how to do generic things in C++-- ideally would like to be able to provide a generic function that euler/rk4/etc is performed
//with generic arguments provided to this function

glm::vec3 euler(SimObject& object, float deltaTime);
glm::vec3 eulerMid(SimObject& object, float deltaTime);
glm::vec3 rk4(SimObject& object, float deltaTime);

//template <typename ReturnType, int numIters>
//struct numericResult_t {
//	std::array<ReturnType, numIters> intermediates;
//	ReturnType result;
//};
//
////these generic numeric method functions return the final result and all intermediate steps
////and the return type of the function being evaluated must be the same as the desired return type of the numeric method (forget for a second that a struct is returned...)
//template<typename ReturnType, typename ...OtherTypes>
//numericResult_t<ReturnType, 1> euler(std::function<ReturnType(OtherTypes...)> func, float deltaTime);
//template<typename ReturnType, typename ...OtherTypes>
//numericResult_t<ReturnType, 2> eulerMid(std::function<ReturnType(OtherTypes...)> func, float deltaTime);
//template<typename ReturnType, typename ...OtherTypes>
//numericResult_t<ReturnType, 4> rk4(std::function<ReturnType(OtherTypes...)> func, float deltaTime);
#endif