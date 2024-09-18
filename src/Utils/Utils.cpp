#include "Utils.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

float roundUpToMultipleOf2(float num)
{
	float roundedNum = std::round(num / 2.0f) * 2.0f;
	return roundedNum;
}

float scaleFloat(float v, float min, float max)
{
	return (max - min) * v + min;
}

float unscaleFloat(float v, float min, float max)
{
	float minValue = min;
	float maxValue = max;
	float originalValue = (v - minValue) / (maxValue - minValue);
	return originalValue;
}

geode::Patch* createPatchSafe(void *address, const geode::ByteVector &data)
{
	auto patch = geode::Mod::get()->patch(address, data);

	if (patch.has_error())
		geode::log::error("Failed to apply patch: {}, data: {}, error: {}", address, data, patch.err());

	return patch.unwrapOr(nullptr);
}

cocos2d::CCRect getScreenSafeArea()
{
	cocos2d::CCRect insets;

	#ifdef GEODE_IS_IOS
	insets = getScreenSafeAreaiOS();
	#endif

	return insets;
}