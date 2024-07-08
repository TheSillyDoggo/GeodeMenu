#include "Utils.hpp"

float roundUpToMultipleOf2(float num)
{
	float roundedNum = std::ceil(num / 2.0f) * 2.0f;
	return roundedNum;
}

float scaleFloat(float v, float min, float max)
{
	return (max - min) * v + min;
}