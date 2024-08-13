#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

#ifdef QOLMOD_ALL_MODES_PLATFORMER

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {    
        auto p = m_isPlatformer;
        m_isPlatformer = false;

        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);

        m_isPlatformer = p;
    }

    QOLMOD_MOD_HOOK("all-plat", "GJBaseGameLayer::collisionCheckObjects")
};

#endif