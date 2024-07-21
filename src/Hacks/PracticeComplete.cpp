#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    struct Fields
    {
        bool hasRespawnedWithCheckpoint;
    };

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
            m_isPracticeMode = m_fields->hasRespawnedWithCheckpoint;

        PlayLayer::levelComplete();
    }

    QOLMOD_MOD_HOOK("practice-complete", "PlayLayer::levelComplete")
};