#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    struct Fields
    {
        bool hasRespawnedWithCheckpoint;

        std::vector<GameObject*> coins;
    };

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (p0->m_objectType == GameObjectType::UserCoin || p0->m_objectType == GameObjectType::SecretCoin) // 142 
        {
            m_fields->coins.push_back(p0);
        }
    }

    void resetLevel()
    {
        m_fields->hasRespawnedWithCheckpoint = false;

        PlayLayer::resetLevel();        
    }

    void loadFromCheckpoint(CheckpointObject* cp)
    {
        m_fields->hasRespawnedWithCheckpoint = true;

        PlayLayer::loadFromCheckpoint(cp);
    }

    void levelComplete()
    {
        if (m_isPracticeMode)
        {
            m_isPracticeMode = m_fields->hasRespawnedWithCheckpoint;

            if (!m_isPracticeMode)
            {
                for (auto coin : m_fields->coins)
                {
                    if (coin->getOpacity() == 0)
                    {
                        pickupItem(as<EffectGameObject*>(coin));
                    }
                }
            }
        }

        PlayLayer::levelComplete();
    }

    QOLMOD_MOD_HOOK("practice-complete", "PlayLayer::levelComplete")
};