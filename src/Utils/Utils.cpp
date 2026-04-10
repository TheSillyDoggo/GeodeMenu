#include "Utils.hpp"
#include <codecvt>

using namespace geode::prelude;

GJGameLevel* qolmod::utils::getCurrentLevel(bool requireLevelData)
{
    if (auto gjbgl = GJBaseGameLayer::get())
        return gjbgl->m_level;

    if (auto scene = CCScene::get())
    {
        if (auto info = scene->getChildByType<LevelInfoLayer>(0))
        {
            return info->m_level;
        }

        if (auto edit = scene->getChildByType<EditLevelLayer>(0))
        {
            return edit->m_level;
        }

        if (!requireLevelData)
        {
            if (auto select = scene->getChildByType<LevelSelectLayer>(0))
            {
                if (auto page = typeinfo_cast<LevelPage*>(select->m_scrollLayer->getPage(select->m_scrollLayer->m_page)))
                {
                    return page->m_level;
                }
            }
        }
    }

    return nullptr;
}

std::wstring qolmod::utils::toWideString(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(str);

    return ws;
}

std::string qolmod::utils::toUTF8String(std::wstring ws)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string str = converter.to_bytes(ws);

    return str;
}

void qolmod::utils::addCircleToNode(CCNode* node, float scale)
{
    auto circle = CCCircleWave::create(0, 25 * scale, 0.2f, true);
    circle->setPosition(node->getContentSize() / 2);
    circle->m_circleMode = CircleMode::Outline;
    circle->setUserObject("allow-circle"_spr, CCNode::create());

    node->addChild(circle, 9999);
}

const unsigned int kSceneFade = 0xFADEFADE;

class fakeCCTransitionCrossFade : public CCTransitionScene
{
    public:
        CCRenderTexture* outTexture = nullptr;
        CCRenderTexture* inTexture = nullptr;

    fakeCCTransitionCrossFade() {}
    ~fakeCCTransitionCrossFade() {}
    
    void updateTransitions(float)
    {
        inTexture->beginWithClear(0, 0, 0, 0);
        m_pInScene->visit();
        inTexture->end();

        outTexture->beginWithClear(0, 0, 0, 0);
        m_pOutScene->visit();
        outTexture->end();
    }

    virtual void draw() {}
    virtual void onEnter()
    {
        CCTransitionScene::onEnter();

        // create a transparent color layer
        // in which we are going to add our rendertextures
        ccColor4B  color = {0,0,0,0};
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCLayerColor* layer = CCLayerColor::create(color);

        // create the first render texture for inScene
        inTexture = CCRenderTexture::create((int)size.width, (int)size.height);

        if (NULL == inTexture)
        {
            return;
        }

        inTexture->getSprite()->setAnchorPoint( ccp(0.5f,0.5f) );
        inTexture->setPosition( ccp(size.width/2, size.height/2) );
        inTexture->setAnchorPoint( ccp(0.5f,0.5f) );

        // render inScene to its texturebuffer
        inTexture->begin();
        m_pInScene->visit();
        inTexture->end();

        // create the second render texture for outScene
        outTexture = CCRenderTexture::create((int)size.width, (int)size.height);
        outTexture->getSprite()->setAnchorPoint( ccp(0.5f,0.5f) );
        outTexture->setPosition( ccp(size.width/2, size.height/2) );
        outTexture->setAnchorPoint( ccp(0.5f,0.5f) );

        // render outScene to its texturebuffer
        outTexture->begin();
        m_pOutScene->visit();
        outTexture->end();

        // create blend functions

        ccBlendFunc blend1 = {GL_ONE, GL_ONE}; // inScene will lay on background and will not be used with alpha
        ccBlendFunc blend2 = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}; // we are going to blend outScene via alpha 

        // set blendfunctions
        inTexture->getSprite()->setBlendFunc(blend1);
        outTexture->getSprite()->setBlendFunc(blend2);    

        // add render textures to the layer
        layer->addChild(inTexture);
        layer->addChild(outTexture);

        // initial opacity:
        inTexture->getSprite()->setOpacity(255);
        outTexture->getSprite()->setOpacity(255);

        // create the blend action
        CCAction* layerAction = CCSequence::create
        (
            CCFadeTo::create(m_fDuration, 0),
            CCCallFunc::create(this, callfunc_selector(CCTransitionScene::hideOutShowIn)),
            CCCallFunc::create(this, callfunc_selector(CCTransitionScene::finish)),
            NULL
        );


        // run the blend action
        outTexture->getSprite()->runAction( layerAction );

        // add the layer (which contains our two rendertextures) to the scene
        addChild(layer, 2, kSceneFade);

        this->schedule(schedule_selector(fakeCCTransitionCrossFade::updateTransitions));
    }
    virtual void onExit()
    {
        this->removeChildByTag(kSceneFade, false);
        CCTransitionScene::onExit();
    }

public:
    static fakeCCTransitionCrossFade* create(float t, CCScene* scene)
    {
        auto* pScene = new fakeCCTransitionCrossFade();
        if(pScene && pScene->initWithDuration(t, scene))
        {
            pScene->autorelease();
            return pScene;
        }
        CC_SAFE_DELETE(pScene);
        return NULL;
    }
};

cocos2d::CCScene* qolmod::utils::createTransitionForEnum(kCCTransition trans, float time, cocos2d::CCScene* scene)
{
    switch (trans)
    {
        default:
            return CCTransitionFade::create(time, scene);
        #if not(GEODE_COMP_GD_VERSION == 22074 && __APPLE__)
        case kCCTransitionCrossFade:
            return fakeCCTransitionCrossFade::create(time, scene);
        case kCCTransitionFadeBL:
            return CCTransitionFadeBL::create(time, scene);
        case kCCTransitionFadeTR:
            return CCTransitionFadeTR::create(time, scene);
        case kCCTransitionFadeUp:
            return CCTransitionFadeUp::create(time, scene);
        case kCCTransitionFadeDown:
            return CCTransitionFadeDown::create(time, scene);
        case kCCTransitionFlipAngular:
            return CCTransitionFlipAngular::create(time, scene);
        case kCCTransitionFlipX:
            return CCTransitionFlipX::create(time, scene);
        case kCCTransitionFlipY:
            return CCTransitionFlipY::create(time, scene);
        case kCCTransitionZoomFlipAngular:
            return CCTransitionZoomFlipAngular::create(time, scene);
        case kCCTransitionZoomFlipX:
            return CCTransitionZoomFlipX::create(time, scene);
        case kCCTransitionZoomFlipY:
            return CCTransitionZoomFlipY::create(time, scene);
        case kCCTransitionJumpZoom:
            return CCTransitionJumpZoom::create(time, scene);
        case kCCTransitionMoveInT:
            return CCTransitionMoveInT::create(time, scene);
        case kCCTransitionMoveInB:
            return CCTransitionMoveInB::create(time, scene);
        case kCCTransitionMoveInL:
            return CCTransitionMoveInL::create(time, scene);
        case kCCTransitionMoveInR:
            return CCTransitionMoveInR::create(time, scene);
        case kCCTransitionRotoZoom:
            return CCTransitionRotoZoom::create(time, scene);
        case kCCTransitionShrinkGrow:
            return CCTransitionShrinkGrow::create(time, scene);
        case kCCTransitionSlideInT:
            return CCTransitionSlideInT::create(time, scene);
        case kCCTransitionSlideInB:
            return CCTransitionSlideInB::create(time, scene);
        case kCCTransitionSlideInL:
            return CCTransitionSlideInL::create(time, scene);
        case kCCTransitionSlideInR:
            return CCTransitionSlideInR::create(time, scene);
        case kCCTransitionSplitRows:
            return CCTransitionSplitRows::create(time, scene);
        case kCCTransitionSplitCols:
            return CCTransitionSplitCols::create(time, scene);
        case kCCTransitionTurnOffTiles:
            return CCTransitionTurnOffTiles::create(time, scene);
        case kCCTransitionProgressRadialCW:
            return CCTransitionProgressRadialCW::create(time, scene);
        case kCCTransitionProgressRadialCCW:
            return CCTransitionProgressRadialCCW::create(time, scene);
        case kCCTransitionProgressHorizontal:
            return CCTransitionProgressHorizontal::create(time, scene);
        case kCCTransitionProgressVertical:
            return CCTransitionProgressVertical::create(time, scene);
        case kCCTransitionProgressInOut:
            return CCTransitionProgressInOut::create(time, scene);
        case kCCTransitionProgressOutIn:
            return CCTransitionProgressOutIn::create(time, scene);
        case kCCTransitionPageTurn:
            return CCTransitionPageTurn::create(time, scene, false);
        #endif
    };
}


std::string qolmod::utils::sizeToPretty(unsigned int size)
{
    double size2 = (double)size;

    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    if (size2 > 0)
    {
        i = std::min(static_cast<int>(std::log(size2) / std::log(1024)), 4);
        size2 /= std::pow(1024, i);
    }
    return fmt::format("{:.2f} {}", size2, suffixes[i]);
}

#ifdef GEODE_IS_ANDROID
extern "C" int __system_property_get(const char* __name, char* __value);
#endif

bool qolmod::utils::isChromebook()
{
    #ifdef GEODE_IS_ANDROID
    #define PROP_VALUE_MAX 92

    static bool chromebook = ([]{
        char device[PROP_VALUE_MAX] = {0};
        __system_property_get("ro.product.device", device);

        std::string s(device);

        return s.find("_cheets") != std::string::npos ||
            s.find("cheets_") != std::string::npos;
    })();

    return chromebook;
    #else
    return false;
    #endif
}

cocos2d::CCRect qolmod::utils::getBasicRect(cocos2d::CCNode* node)
{
    return CCRectMake(node->m_obPosition.x - node->m_obContentSize.width * node->m_obAnchorPoint.x,
        node->m_obPosition.y - node->m_obContentSize.height * node->m_obAnchorPoint.y,
        node->m_obContentSize.width, node->m_obContentSize.height);
}