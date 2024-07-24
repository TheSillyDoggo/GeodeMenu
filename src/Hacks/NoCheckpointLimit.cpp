#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    void storeCheckpoint(CheckpointObject* cp)
    {
        m_checkpointArray->addObject(cp);

        PlayLayer::addToSection(cp->m_physicalCheckpointObject);
    }

    QOLMOD_MOD_ALL_HOOKS("no-checkpoint-limit")
};
