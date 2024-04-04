#ifndef GEODE_IS_ANDROID64

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* allMod = nullptr;

class $modify (GJBaseGameLayer)
{
    TodoReturn collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        if (!allMod)
            allMod = Client::GetModule("all-plat");

        if (allMod->enabled)
        {
            #ifdef GEODE_IS_WINDOWS
            auto* value = reinterpret_cast<uintptr_t*>(((uintptr_t)this) + 0x29C6);
            #endif

            #ifdef GEODE_IS_ANDROID32
            auto* value = reinterpret_cast<uintptr_t*>(((uintptr_t)this) + 0x29e6);
            #endif

            //#ifdef GEODE_IS_ANDROID64
            //auto* value = reinterpret_cast<uintptr_t*>(((uintptr_t)this) + 0x29C6);
            //#endif
            
            uintptr_t og = *value;
            *value = 0;
            GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
            *value = og;
        }
        else
        {
            GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
        }
    }
};

#endif