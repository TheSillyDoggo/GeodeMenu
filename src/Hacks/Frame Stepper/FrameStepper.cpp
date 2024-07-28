/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

bool enabled()
{
    return CCKeyboardDispatcher::get()->getShiftKeyPressed();
}

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        if (enabled())
            return GJBaseGameLayer::update(p0);
    }
};*/