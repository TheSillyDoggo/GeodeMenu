#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCNode)
{
    void setRotation(float r)
    {
        if (Client::GetModuleEnabled("no-rot"))
            CCNode::setRotation(0);
        else
            CCNode::setRotation(r);
    }
};