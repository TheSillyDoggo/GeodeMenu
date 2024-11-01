#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

InputModule* hitPlayer;
InputModule* hitSolid;
InputModule* hitHazard;

/*
hitbox-multiplier-player
hitbox-multiplier-solid
hitbox-multiplier-hazard
*/

class $modify (GameObject)
{
    virtual cocos2d::CCRect getObjectRect(float p0, float p1)
    {
        if (!hitPlayer)
        {
            if (auto hitboxMult = Client::GetModule("hitbox-multiplier"))
            {
                hitPlayer = as<InputModule*>(hitboxMult->options[0]);
                hitSolid = as<InputModule*>(hitboxMult->options[2]);
                hitHazard = as<InputModule*>(hitboxMult->options[4]);
            }
        }

        if (typeinfo_cast<PlayerObject*>(this))
        {
            p0 *= hitPlayer->getFloatValue();
            p1 *= hitPlayer->getFloatValue();
        }
        else
        {
            if (m_objectType == GameObjectType::Solid || m_objectType == GameObjectType::Slope)
            {
                p0 *= hitSolid->getFloatValue();
                p1 *= hitSolid->getFloatValue();
            }
            
            if (m_objectType == GameObjectType::Hazard || m_objectType == GameObjectType::AnimatedHazard)
            {
                p0 *= hitHazard->getFloatValue();
                p1 *= hitHazard->getFloatValue();
            }
        }

        return GameObject::getObjectRect(p0, p1);
    }

    QOLMOD_MOD_ALL_HOOKS("hitbox-multiplier")
};