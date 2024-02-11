/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual void setVisible(bool p0)
    {
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) // thanks firee (in the hole) for the practice checkpoint id
            GameObject::setVisible(false);
        else
            GameObject::setVisible(true);
    }

    virtual void setOpacity(unsigned char p0)
    {
        GameObject::setOpacity(255);
    }
};

class $modify (GJBaseGameLayer)
{
    virtual TodoReturn updateColor(cocos2d::ccColor3B& p0, float p1, int p2, bool p3, float p4, cocos2d::ccHSVValue& p5, int p6, bool p7, EffectGameObject* p8, int p9, int p10)
    {

    }
};*/