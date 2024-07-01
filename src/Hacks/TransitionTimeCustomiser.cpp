#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/CCDirector.hpp>
#include "../Client/Client.h"
#include "../Utils/defines.hpp"

using namespace geode::prelude;

#ifndef GEODE_IS_APPLE

//{"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular" "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"}

CCScene* getSceneForSel(int i, float f, CCScene* s)
{
    switch (i)
    {
        default:
            return CCTransitionFade::create(f, s);
        case 1:
            return CCTransitionCrossFade::create(f, s);
        case 2:
            return CCTransitionFadeBL::create(f, s);
        case 3:
            return CCTransitionFadeTR::create(f, s);
        case 4:
            return CCTransitionFadeUp::create(f, s);
        case 5:
            return CCTransitionFadeDown::create(f, s);
        case 6:
            return CCTransitionFlipAngular::create(f, s);
        case 7:
            return CCTransitionFlipX::create(f, s);
        case 8:
            return CCTransitionFlipY::create(f, s);
        case 9:
            return CCTransitionZoomFlipAngular::create(f, s);
        case 10:
            return CCTransitionZoomFlipX::create(f, s);
        case 11:
            return CCTransitionZoomFlipY::create(f, s);
        case 12:
            return CCTransitionJumpZoom::create(f, s);
        case 13:
            return CCTransitionMoveInT::create(f, s);
        case 14:
            return CCTransitionMoveInB::create(f, s);
        case 15:
            return CCTransitionMoveInL::create(f, s);
        case 16:
            return CCTransitionMoveInR::create(f, s);
        case 17:
            return CCTransitionRotoZoom::create(f, s);
        case 18:
            return CCTransitionShrinkGrow::create(f, s);
        case 19:
            return CCTransitionSlideInT::create(f, s);
        case 20:
            return CCTransitionSlideInB::create(f, s);
        case 21:
            return CCTransitionSlideInL::create(f, s);
        case 22:
            return CCTransitionSlideInR::create(f, s);
        case 23:
            return CCTransitionSplitRows::create(f, s);
        case 24:
            return CCTransitionSplitCols::create(f, s);
        case 25:
            return CCTransitionTurnOffTiles::create(f, s);
    }
}

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration,CCScene* scene)
    {
        float mod = 1;

        if (Client::GetModuleEnabled("no-trans"))
        {
            mod = 0;
        }

        if (SpeedhackTop::instance && SpeedhackTransFix::instance->enabled)
        {
            if (SpeedhackEnabled::instance->enabled)
            {
                float v = SpeedhackTop::instance->getFloatValue();

                v /= CCDirector::get()->getScheduler()->getTimeScale();

                mod = mod * v;
            }
        }

        return as<CCTransitionFade*>(getSceneForSel(Client::GetModuleEnabled("custom-trans") ? Mod::get()->getSavedValue<int>("transition", 0) : 0, duration * mod, scene));// /*base_cast<CCTransitionFade*>(CCTransitionFlipY::create(duration * mod, scene));  */CCTransitionFade::create(duration * mod, scene);
    }
};

#else

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration,CCScene* scene)
    {
        return CCTransitionFade::create((Client::GetModuleEnabled("no-trans") ? 0 : duration) * (SpeedhackTransFix::instance->enabled ? (SpeedhackEnabled::instance->enabled ? (1 / SpeedhackTop::instance->getFloatValue()) : 1) : 1), scene);
    }
};

#endif