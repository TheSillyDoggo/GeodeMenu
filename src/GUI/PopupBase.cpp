#include "PopupBase.hpp"
#include <BlurAPI.hpp>
#include "Modules/BlurBackground.hpp"
#include "../Utils/RealtimeAction.hpp"

using namespace geode::prelude;

bool PopupBase::init(float width, float height)
{
    if (!CCLayerColor::init())
        return false;

    if (BlurMenuBG::get()->getRealEnabled())
    {
        blurLayer = CCLayerColor::create(ccc4(0, 0, 0, 0));
        BlurAPI::addBlur(blurLayer);

        this->addChild(blurLayer, -69);
    }

    CCTouchDispatcher::get()->registerForcePrio(this, 2);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);

    m_size = ccp(width, height);
    m_bgSprite = CCScale9Sprite::create("GJ_square01.png");
    m_buttonMenu = CCMenu::create();

    m_mainLayer = CCLayer::create();
    m_mainLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->setContentSize(m_size);
    m_mainLayer->setAnchorPoint(ccp(0.5f, 0.5f));
    m_mainLayer->setPosition(getContentSize() / 2);

    setup();
    this->addChild(m_mainLayer);
    this->addChild(m_buttonMenu);
    this->addChild(m_bgSprite);
    return true;
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
    scene->addChild(this, scene->getHighestChildZ() + 1);

    if (!m_noElasticity)
    {
        m_mainLayer->setScale(0);
        m_mainLayer->runAction(RealtimeAction::create(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1.0f), 0.6f)));
        this->runAction(RealtimeAction::create(CCFadeTo::create(0.14f, 150)));
    }
}

PopupBase::~PopupBase()
{
    CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void PopupBase::registerWithTouchDispatcher()
{
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}