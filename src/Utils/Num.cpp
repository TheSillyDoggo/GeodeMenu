#include "Num.hpp"
#include <Geode/Geode.hpp>

// brings num from between 0 - 1 to between min - max
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


std::string floatToStringMin2DP(double value)
{
    auto str = geode::utils::numToString(value, 6);

	for (size_t i = 0; i < 6 - 2; i++)
	{
		if (str.ends_with("0"))
			str = str.substr(0, str.size() - 1);
	}
	
	return str;
}

using namespace geode::prelude;

CCNode* getTopLevelNonSceneNode(CCNode* node)
{
	auto n = node;

	int i = 0;
	while (n && n->getParent())
	{
		if (typeinfo_cast<CCScene*>(n->getParent()) || i > 100)
			break;
		
		n = n->getParent();
		i++;
	}

	return n;
}

CCRect getWorldSpaceBoundingBox(CCNode* node)
{
	auto parent = node->getParent();
	auto bounding_box = node->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

	return CCRectMake(min.x, min.y, max.x - min.x, max.y - min.y);
}

void getAllChildrenRecursive(CCNode* node, std::vector<CCNode*>& outChildren)
{
	if (!node) return;

	CCArray* children = node->getChildren();
	if (children && children->count() > 0) {
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(children, pObj) {
			CCNode* child = dynamic_cast<CCNode*>(pObj);
			if (child) {
				outChildren.push_back(child);
				getAllChildrenRecursive(child, outChildren);
			}
		}
	}
}