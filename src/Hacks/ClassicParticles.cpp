/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        for (auto system : CCArrayExt<CCParticleSystem*>(m_particleSystems))
        {
            system->setPositionType(cocos2d::tCCPositionType::kCCPositionTypeGrouped);
        }
    }
};*/