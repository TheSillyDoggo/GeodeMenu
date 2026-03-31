#include "PopupBase.hpp"
#include <BlurAPI.hpp>
#include "Modules/BlurBackground.hpp"
#include "../Utils/RealtimeAction.hpp"
#include "Modules/PopupUIScale.hpp"

using namespace geode::prelude;
using namespace qolmod;

bool PopupBase::init(float width, float height)
{
    if (!CCLayerColor::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));

    rt = CCRenderTexture::create(
        getContentWidth(),
        getContentHeight(),
        kCCTexture2DPixelFormat_RGBA8888,
        GL_DEPTH24_STENCIL8
    );
    rt->getSprite()->setAnchorPoint(ccp(0, 1));
    rt->getSprite()->setBlendFunc(this->getBlendFunc());
    #if GEODE_COMP_GD_VERSION >= 22081
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    #endif

    CCTouchDispatcher::get()->registerForcePrio(this, 2);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);

    m_size = ccp(width, height);
    m_bgSprite = CCScale9Sprite::create("GJ_square01.png");
    m_bgSprite->setVisible(false);
    m_buttonMenu = CCMenu::create();

    m_mainLayer = CCLayer::create();
    m_mainLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->setContentSize(m_size);
    m_mainLayer->setAnchorPoint(ccp(0.5f, 0.5f));
    m_mainLayer->setPosition(getContentSize() / 2);

    updateBlur();
    updateUIScale();
    setup();
    this->addChild(m_mainLayer);
    this->addChild(m_buttonMenu);
    this->addChild(m_bgSprite);
    return true;
}

// short for calculator btw
float PopupBase::calcPopupScale()
{
    return std::clamp<float>(PopupUIScale::get()->getValue(), 0.6f, 1);
}

void PopupBase::updateUIScale()
{
    float uiScale = calcPopupScale();

    this->setScale(uiScale);
    this->setContentSize(CCDirector::get()->getWinSize() / uiScale);

    if (blurLayer)
        blurLayer->setContentSize(getContentSize());

    m_mainLayer->setPosition(getContentSize() / 2);
    updateLayout();
}

void PopupBase::updateBlur()
{
    if (blurLayer)
    {
        blurLayer->removeFromParent();
        blurLayer = nullptr;
    }

    if (BlurMenuBG::get()->getRealEnabled() && BlurAPI::isBlurAPIEnabled())
    {
        blurLayer = CCLayerColor::create(ccc4(0, 0, 0, 0));
        blurLayer->setContentSize(getContentSize());
        BlurAPI::addBlur(blurLayer);

        this->addChild(blurLayer, -69);
    }
}

bool PopupBase::initAnchored(float width, float height)
{
    return init(width, height);
}

bool PopupBase::setup()
{
    return true;
}

void PopupBase::onClose(CCObject* sender)
{
    this->removeFromParent();
}

void PopupBase::onBtn1(CCObject* sender)
{
    this->removeFromParent();
}

void PopupBase::onBtn2(CCObject* sender)
{
    this->removeFromParent();
}

void PopupBase::keyBackClicked()
{
    onClose(nullptr);
}

void PopupBase::show()
{
    auto scene = CCScene::get();
    if (addToScene)
        scene->addChild(this, scene->getHighestChildZ() + 1);

    if (!m_noElasticity)
    {
        this->playAnimation((MenuAnimation)Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale));
    }
}

std::vector<PopupBase*> allPopups = {};

void PopupBase::updateAllUIScale()
{
    for (auto popup : allPopups)
    {
        popup->updateUIScale();
    }
}

void PopupBase::updateAllBlur()
{
    for (auto popup : allPopups)
    {
        popup->updateBlur();
    }
}

PopupBase::PopupBase()
{
    allPopups.push_back(this);
}

PopupBase::~PopupBase()
{
    allPopups.erase(std::find(allPopups.begin(), allPopups.end(), this));
    CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void PopupBase::registerWithTouchDispatcher()
{
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}

void PopupBase::keyDown(cocos2d::enumKeyCodes code)
{
    this->keyDown(code, 0);
}

void PopupBase::keyDown(cocos2d::enumKeyCodes code, double timestamp)
{
    #if GEODE_COMP_GD_VERSION >= 22081
    CCLayerColor::keyDown(code, timestamp);
    #else
    CCLayerColor::keyDown(code);
    #endif
}

void PopupBase::visit()
{
    static bool drawingOriginal = false;

    if (!shouldVisit)
        return;

    if (drawOpacity == 1.0f || drawingOriginal)
        return CCLayerColor::visit();

    rt->beginWithClear(0, 0, 0, 0);

    auto op = getOpacity();
    setOpacity(0);
    drawingOriginal = true;
    CCLayerColor::visit();
    drawingOriginal = false;
    setOpacity(op);

    rt->end();

    if (blurLayer)
        rt->setScale((float)rt->m_nWidth / CCEGLView::get()->getFrameSize().width);
    
    rt->getSprite()->setOpacity(drawOpacity * 255.0f);
    CCLayerColor::draw();
    rt->visit();
}

void PopupBase::draw()
{
    if (!blurLayer)
        CCLayerColor::draw();
}

void PopupBase::updateTweenAction(float value, const char* key)
{
    if (std::string(key) == "drawOpacity")
        this->drawOpacity = value;
}

void PopupBase::playAnimation(MenuAnimation anim)
{
    drawOpacity = 1;

    auto winSize = getContentSize();
    auto moveToMid = RealtimeAction::create(CCSequence::create(CCEaseElasticOut::create(CCMoveTo::create(1, getContentSize() / 2), 0.8f), nullptr));
    auto fadeInBG = RealtimeAction::create(CCFadeTo::create(0.14f, baseOpacity));

    this->stopAllActions();
    m_mainLayer->stopAllActions();
    m_mainLayer->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    m_mainLayer->setScale(1.0f);
    this->setOpacity(baseOpacity);

    this->setOpacity(0);
    this->runAction(fadeInBG);

    switch (anim)
    {
        case MenuAnimation::None:
            this->stopAction(fadeInBG);
            this->setOpacity(baseOpacity);
            return;

        case MenuAnimation::FromTop:
            m_mainLayer->setPosition(ccp(winSize.width / 2, winSize.height + m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromBottom:
            m_mainLayer->setPosition(ccp(winSize.width / 2, -m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromLeft:
            m_mainLayer->setPosition(ccp(-m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromRight:

            m_mainLayer->setPosition(ccp(winSize.width + m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::Scale:
            m_mainLayer->setScale(0);
            m_mainLayer->runAction(RealtimeAction::create(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1.0f), 0.6f)));
            return;

        case MenuAnimation::FadeIn:
            drawOpacity = 0;
            this->runAction(
                RealtimeAction::create(
                CCEaseOut::create(
                CCActionTween::create(0.25f, "drawOpacity", 0, 1),
                2.0f // easing amount
            )));
            return;
    }
}