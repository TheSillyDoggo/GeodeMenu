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
    }

    return false;
}