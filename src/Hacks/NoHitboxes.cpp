/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual cocos2d::CCRect const& getObjectRect()
    {
        if (Module::get("no-hitboxes")->enabled)
            this->m_isNoTouch = true;            

        return GameObject::getObjectRect();
    }
};*/