#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <vector>
#include <glm/glm.hpp>

template <typename ReturnType>
class MathFunction {
public:
	std::vector<float> floatArgs;
	std::vector<glm::vec3> vecArgs;

	template <typename... Args>
	void evaluate(float first, Args... rest);
	template <typename... Args>
	void evaluate(double first, Args... rest);
	template <typename... Args>
	void evaluate(glm::vec3 first, Args... rest);
	ReturnType evaluate();
};

#endif