#include "AndroidUI.hpp"
#include "../Client/Module.hpp"

bool AndroidUI::setup()
{
    auto btn = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("diffIcon_02_btn_001.png"), CCSprite::createWithSpriteFrameName("diffIcon_04_btn_001.png"), this, menu_selector(AndroidUI::test));
    btn->toggle(Module::getByID("noclip")->getUserEnabled());

    m_buttonMenu->addChild(btn);

    return true;
}

AndroidUI* AndroidUI::create()
{
    auto pRet = new AndroidUI();

    if (pRet && pRet->initAnchored(300.f, 300.f))
    {
        PlatformToolbox::showCursor();
        instance = pRet;

        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AndroidUI* AndroidUI::addToScene()
{
    auto pRet = create();
    pRet->show();

    return pRet;
}

AndroidUI* AndroidUI::get()
{
    return instance;
}

AndroidUI::~AndroidUI()
{
    instance = nullptr;
}

void AndroidUI::close()
{
    if (PlayLayer::get() && !PlayLayer::get()->m_isPaused && !PlayLayer::get()->m_levelEndAnimationStarted && !GameManager::sharedState()->getGameVariable("0024"))
        PlatformToolbox::hideCursor();

    this->onClose(nullptr);
}

void AndroidUI::test(CCObject* sender)
{
    auto mod = Module::getByID("noclip");

    mod->setUserEnabled(!mod->getUserEnabled());
}