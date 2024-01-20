/*#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <Geode/modify/CCTransitionFade.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration,CCScene* scene)
    {
        float mod = 1;

        if (Client::GetModuleEnabled("no-trans"))
        {
            mod = 0;
        }

        return CCTransitionFade::create(duration * mod, scene);
    }
};*/