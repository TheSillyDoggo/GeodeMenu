#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify (CustomEndLevelLayer, EndLevelLayer)
{
    virtual void customSetup();
};