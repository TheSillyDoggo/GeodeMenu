#include "HitboxUtils.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool HitboxUtils::shouldHitboxesBeVisible()
{
    if (auto gjbgl = GJBaseGameLayer::get())
    {
        if (gjbgl->m_isDebugDrawEnabled)
        {
            if (auto pl = typeinfo_cast<PlayLayer*>(gjbgl))
            {
                if (pl->m_isPracticeMode)
                    return true;
            }
            else
            {
                return true;
            }
        }
        
        if (gjbgl->m_hitboxesOnDeath)
        {
            if (gjbgl->m_player1 && gjbgl->m_player1->m_isDead)
                return true;

            if (gjbgl->m_player2 && gjbgl->m_player2->m_isDead)
                return true;
        }
    }

    return false;
}