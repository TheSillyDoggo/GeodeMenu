#pragma once

#include <string>
#include <Geode/Geode.hpp>

float scaleFloat(float v, float min, float max);
float unscaleFloat(float v, float min, float max);

std::string floatToStringMin2DP(double value);

cocos2d::CCNode* getTopLevelNonSceneNode(cocos2d::CCNode* node);
cocos2d::CCRect getWorldSpaceBoundingBox(cocos2d::CCNode* node);
void getAllChildrenRecursive(cocos2d::CCNode* node, std::vector<cocos2d::CCNode*>& outChildren);