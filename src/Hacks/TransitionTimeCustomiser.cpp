#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/CCDirector.hpp>
#include "../Client/Client.h"
#include "../Utils/defines.hpp"

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

    return CCTransitionFade::create(f, s);

    switch (i)
    {
        default:
            return CCTransitionFade::create(f, s);
        case 1:
            return CCTransitionCrossFade::create(f, s); // m1 0xc3a80, imac 0xdcd50
        case 2:
            return CCTransitionFadeBL::create(f, s); // m1 0xc4a64, imac 0xdde50
        case 3:
            return CCTransitionFadeTR::create(f, s); // m1 0xc4784, imac 0xddb20
        case 4:
            return CCTransitionFadeUp::create(f, s); // m1 0xc4c2c, imac 0xde050
        case 5:
            return CCTransitionFadeDown::create(f, s); // m1 0xc4df4, imac 0xde250
        case 6:
            return CCTransitionFlipAngular::create(f, s); // m1 0xc2900, imac 0xdba50
        case 7:
            return CCTransitionFlipX::create(f, s); // m1 0xc21dc, imac 0xdb290
        case 8:
            return CCTransitionFlipY::create(f, s); // m1 0xc256c, imac 0xdb670
        case 9:
            return CCTransitionZoomFlipAngular::create(f, s); // 
        case 10:
            return CCTransitionZoomFlipX::create(f, s); // 
        case 11:
            return CCTransitionZoomFlipY::create(f, s); // 
        case 12:
            return CCTransitionJumpZoom::create(f, s); // m1 0xc0424, imac 0xd92a0
        case 13:
            return CCTransitionMoveInT::create(f, s); // 
        case 14:
            return CCTransitionMoveInB::create(f, s); // m1 0xc0f24, imac 0xd9e80
        case 15:
            return CCTransitionMoveInL::create(f, s); // m1 0xc07b8, imac 0xd9690
        case 16:
            return CCTransitionMoveInR::create(f, s); // m1 0xc0afc, imac 0xd9a00
        case 17:
            return CCTransitionRotoZoom::create(f, s);
        case 18:
            return CCTransitionShrinkGrow::create(f, s); // 
        case 19:
            return CCTransitionSlideInT::create(f, s); // 
        case 20:
            return CCTransitionSlideInB::create(f, s); // 
        case 21:
            return CCTransitionSlideInL::create(f, s); // 
        case 22:
            return CCTransitionSlideInR::create(f, s); // 
        case 23:
            return CCTransitionSplitRows::create(f, s); // 
        case 24:
            return CCTransitionSplitCols::create(f, s); // 
        case 25:
            return CCTransitionTurnOffTiles::create(f, s); // 
    }
}

class $modify (cocos2d::CCTransitionFade)
{
    static CCTransitionFade* create(float duration, CCScene* scene)
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

        return as<CCTransitionFade*>(getSceneForSel(Client::GetModuleEnabled("custom-trans") ? Mod::get()->getSavedValue<int>("transition", 0) : 0, duration * mod, scene, ccc3(0, 0, 0)));// /*base_cast<CCTransitionFade*>(CCTransitionFlipY::create(duration * mod, scene));  */CCTransitionFade::create(duration * mod, scene);
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