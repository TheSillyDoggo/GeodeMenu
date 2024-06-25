#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace PCDrawUtils
{
    void drawRect(CCPoint pos, CCPoint size, ccColor4B colour);

    CCPoint getMousePosition();
};