#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual void setVisible(bool visible)
    {
        if (Client::GetModuleEnabled("obj-vis"))
            GameObject::setVisible(true);
        else
            GameObject::setVisible(visible);
    }
};

class $modify (CCNode)
{
    virtual void setVisible(bool visible)
    {
        if (Client::GetModuleEnabled("node-vis"))
            CCNode::setVisible(true);
        else
            CCNode::setVisible(visible);
    }
};