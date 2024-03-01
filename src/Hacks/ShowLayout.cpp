/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual void setVisible(bool p0)
    {
        if (PlayLayer::get())
        {
            if (m_objectType == GameObjectType::Decoration && m_objectID != 44) // thanks firee (in the hole) for the practice checkpoint id
                GameObject::setVisible(false);
            else
                GameObject::setVisible(true);
        }
        else
            GameObject::setVisible(p0);
    }

    virtual void setOpacity(unsigned char p0)
    {
        GameObject::setOpacity(255);
    }

    virtual void setChildColor(cocos2d::ccColor3B const& p0)
    {

    }
};*/

/*
class $modify(GameObject) {
    void setVisible(bool v) {
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        //m_hasGroupParent == 0
        std::vector<int> outerPortal = {};
        //i really dont want to have to check every single object id
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
    }
};*/