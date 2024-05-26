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
    AppDelegate::get()->willSwitchToScene(s);

    if (i == 0)
        return CCTransitionFade::create(f, s);
    //#ifndef GEODE_IS_MACOS
    else if (i == 1)
        return CCTransitionCrossFade::create(f, s);
    else if (i == 2)
        return CCTransitionFadeBL::create(f, s);
    else if (i == 3)
        return CCTransitionFadeTR::create(f, s);
    else if (i == 4)
        return CCTransitionFadeUp::create(f, s);
    else if (i == 5)
        return CCTransitionFadeDown::create(f, s);
    else if (i == 6)
        return CCTransitionFlipAngular::create(f, s);
    else if (i == 7)
        return CCTransitionFlipX::create(f, s);
    else if (i == 8)
        return CCTransitionFlipY::create(f, s);
    else if (i == 9)
        return CCTransitionZoomFlipAngular::create(f, s);
    else if (i == 10)
        return CCTransitionZoomFlipX::create(f, s);
    else if (i == 11)
        return CCTransitionZoomFlipY::create(f, s);
    else if (i == 12)
        return CCTransitionJumpZoom::create(f, s);
    else if (i == 13)
        return CCTransitionMoveInT::create(f, s);
    else if (i == 14)
        return CCTransitionMoveInB::create(f, s);
    else if (i == 15)
        return CCTransitionMoveInL::create(f, s);
    else if (i == 16)
        return CCTransitionMoveInR::create(f, s);
    else if (i == 17)
        return CCTransitionRotoZoom::create(f, s);
    else if (i == 18)
        return CCTransitionShrinkGrow::create(f, s);
    else if (i == 19)
        return CCTransitionSlideInT::create(f, s);
    else if (i == 20)
        return CCTransitionSlideInB::create(f, s);
    else if (i == 21)
        return CCTransitionSlideInL::create(f, s);
    else if (i == 22)
        return CCTransitionSlideInR::create(f, s);
    else if (i == 23)
        return CCTransitionSplitRows::create(f, s);
    else if (i == 24)
        return CCTransitionSplitCols::create(f, s);
    else if (i == 25)
        return CCTransitionTurnOffTiles::create(f, s);
    
    //#endif

    return CCTransitionFade::create(f, s);
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

        float v = 1.0f;

        if (SpeedhackTop::instance && SpeedhackTransFix::instance->enabled)
        {
            if (SpeedhackEnabled::instance->enabled)
            {
                auto x = numFromString<float>(SpeedhackTop::instance->text);

                if (x.isOk())
                {
                    v = x.value();
                    
                    if (v < 0.01f)
                        v = 0.01f;

                    if (v > 99999)
                        v = 99999;
                }
                else
                    v = 1;

                bool m = SpeedhackMus::instance->enabled;

                if (SpeedhackGameplay::instance->enabled)
                    if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

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