#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/CCDirector.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

//{"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular" "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"}

class TransCust {
    public:
        static inline bool v = false;
};

CCScene* g(CCTransitionScene* n)
{
    TransCust::v = false;

    return n;
}

CCScene* getSceneForSel(int i, float f, CCScene* s)
{
    TransCust::v = true;
    AppDelegate::get()->willSwitchToScene(s);

    if (i == 0)
        return g(CCTransitionFade::create(f, s));
    else if (i == 1)
        return g(CCTransitionCrossFade::create(f, s));
    else if (i == 2)
        return g(CCTransitionFadeBL::create(f, s));
    else if (i == 3)
        return g(CCTransitionFadeTR::create(f, s));
    else if (i == 4)
        return g(CCTransitionFadeUp::create(f, s));
    else if (i == 5)
        return g(CCTransitionFadeDown::create(f, s));
    else if (i == 6)
        return g(CCTransitionFlipAngular::create(f, s));
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
        return g(CCTransitionTurnOffTiles::create(f, s));

    return nullptr;
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

        return as<CCTransitionFade*>(getSceneForSel(Mod::get()->getSavedValue<int>("transition", 0), duration * mod, scene));// /*base_cast<CCTransitionFade*>(CCTransitionFlipY::create(duration * mod, scene));  */CCTransitionFade::create(duration * mod, scene);
    }
};

bool myInitWW(CCRenderTexture* ins, int w, int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat) {
    log::info("ASDF: {} | {}", w, h);

    if (TransCust::v)    
        return ins->initWithWidthAndHeight(CCEGLView::get()->getFrameSize().width, CCEGLView::get()->getFrameSize().height, eFormat, uDepthStencilFormat);
    else
        return ins->initWithWidthAndHeight(w, h, eFormat, uDepthStencilFormat);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                geode::modifier::Resolve<int, int, CCTexture2DPixelFormat, GLuint>::func(&CCRenderTexture::initWithWidthAndHeight)
            )
            //geode::addresser::getNonVirtual(&CCRenderTexture::initWithWidthAndHeight)
        ),
        &myInitWW,
        "cocos2d::CCRenderTexture::initWithWidthAndHeight",
        tulip::hook::TulipConvention::Thiscall
    );
}