#pragma once

#include <Geode/Geode.hpp>

namespace qolmod::utils
{
    GJGameLevel* getCurrentLevel(bool requireLevelData = false);
    std::wstring toWideString(std::string str);
    std::string toUTF8String(std::wstring ws);
    void addCircleToNode(cocos2d::CCNode* node , float scale = 1.0f);
};