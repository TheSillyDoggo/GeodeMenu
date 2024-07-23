#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    struct Fields
    {
        std::vector<EffectGameObject*> coins;
    };

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (p0->m_objectType == GameObjectType::UserCoin || p0->m_objectType == GameObjectType::SecretCoin) // 142 
        {
           m_fields->coins.push_back(as<EffectGameObject*>(p0));
        }
    }

    void resetLevel()
    {
        PlayLayer::resetLevel();

        for (auto coin : m_fields->coins)
        {
            pickupItem(coin);
            destroyObject(coin);
        }
    }

    QOLMOD_MOD_ALL_HOOKS("auto-coins")
};