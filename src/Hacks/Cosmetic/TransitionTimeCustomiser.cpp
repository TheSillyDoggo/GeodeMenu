#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <../../../build/bindings/bindings/Geode/modify/CCTransitionFade.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCDirector.hpp>
#include "../../Client/Client.h"
#include "../../Utils/defines.hpp"

using namespace geode::prelude;

#ifndef GEODE_IS_IOS

//{"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular" "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"}

#define TRANS(__trans__) \
ind++; \
if (i == ind) \
    return __trans__::create(f, s)

CCScene* getSceneForSel(int i, float f, CCScene* s, const ccColor3B& color)
{
    AppDelegate::get()->willSwitchToScene(s);

    int ind = 0;

    #ifdef QOLMOD_TRANSITION_CUSTOMIZER

    TRANS(CCTransitionCrossFade);
    TRANS(CCTransitionFadeBL);
    TRANS(CCTransitionFadeTR);
    TRANS(CCTransitionFadeUp);
    TRANS(CCTransitionFadeDown);
    TRANS(CCTransitionFlipAngular);
    TRANS(CCTransitionFlipX);
    TRANS(CCTransitionFlipY);
    TRANS(CCTransitionZoomFlipAngular);
    TRANS(CCTransitionZoomFlipX);
    TRANS(CCTransitionZoomFlipY);
    TRANS(CCTransitionJumpZoom);
    TRANS(CCTransitionMoveInT);
    TRANS(CCTransitionMoveInB);
    TRANS(CCTransitionMoveInL);
    TRANS(CCTransitionMoveInR);
    TRANS(CCTransitionRotoZoom);
    TRANS(CCTransitionShrinkGrow);
    TRANS(CCTransitionSlideInT);
    TRANS(CCTransitionSlideInB);
    TRANS(CCTransitionSlideInL);
    TRANS(CCTransitionSlideInR);
    TRANS(CCTransitionSplitRows);
    TRANS(CCTransitionSplitCols);
    TRANS(CCTransitionTurnOffTiles);

    #endif

    return CCTransitionFade::create(f, s);
}

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration, CCScene* scene)
    {
        float mod = 1;

        if (Client::GetModuleEnabled("custom-trans"))
        {
            duration = Mod::get()->getSavedValue<float>("transition-time", 0.5f);
        }

        if (Client::GetModuleEnabled("no-trans"))
        {
            mod = 0;
        }

        return as<CCTransitionFade*>(getSceneForSel(Client::GetModuleEnabled("custom-trans") ? Mod::get()->getSavedValue<int>("transition", 0) : 0, duration * mod, scene, ccc3(0, 0, 0)));// /*base_cast<CCTransitionFade*>(CCTransitionFlipY::create(duration * mod, scene));  */CCTransitionFade::create(duration * mod, scene);
    }
};

#else

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration,CCScene* scene)
    {
        return CCTransitionFade::create(Client::GetModuleEnabled("no-trans") ? 0 : duration, scene);
    }
};

#endif