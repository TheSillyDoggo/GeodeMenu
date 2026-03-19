#include "Hooks.hpp"
#include "CheckpointFix.hpp"

void FixedPlayerObject::loadFromCheckpoint(PlayerCheckpoint* checkpoint)
{
    PlayerObject::loadFromCheckpoint(checkpoint);

    if (CheckpointFix::get()->getRealEnabled())
        static_cast<FixedPlayerCheckpoint*>(checkpoint)->m_fields->state.loadState(this);
}

void FixedPlayerObject::saveToCheckpoint(PlayerCheckpoint* checkpoint)
{
    PlayerObject::saveToCheckpoint(checkpoint);

    static_cast<FixedPlayerCheckpoint*>(checkpoint)->m_fields->state.saveState(this);
}