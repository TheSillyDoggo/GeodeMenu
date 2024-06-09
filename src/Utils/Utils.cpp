#include "Utils.hpp"

float roundUpToMultipleOf2(float num) {
	float roundedNum = std::ceil(num / 2.0f) * 2.0f;
	return roundedNum;
}