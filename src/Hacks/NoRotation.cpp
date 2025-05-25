#ifdef NO_ROTATION_BUILD

#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* rot = nullptr;

class $modify (PlayerObject)
{
    void setRotation(float r)
    {
        if (!rot)
            rot = Module::get("no-rot");

        if (rot->enabled)
            PlayerObject::setRotation(0);
        else
            PlayerObject::setRotation(r);
    }
};

#endif

/*
class $modify (CCNode)
{
    void setRotation(float r)
    {
        if (Module::get("no-rot")->enabled)
            CCNode::setRotation(0);
        else
            CCNode::setRotation(r);
    }
};*/