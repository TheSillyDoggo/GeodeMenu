#include "AndroidBall.h"
#include "../Utils/UnspeedhackedAction.hpp"

AndroidBall* AndroidBall::get()
{
    return instance;
}

bool AndroidBall::init()
{
    if (!CCLayer::init())
        return false;

    this->setID("QOLModButton"_spr);
    this->setMouseEnabled(false);
    this->setTouchEnabled(false);

    instance = this;

    mod = Module::get("hide-btn");
    mod2 = Module::get("instant-fade");
    canDrag = Module::get("allow-dragging");

    menu = CCMenu::create();
    menu->setPosition(position);
    menu->setContentSize(ccp(0, 0));

    //l = CCLabelBMFont::create(">_", "bigFont.fnt");
    //l->setAnchorPoint(ccp(0.5f, 0.35f));

    btnOverlay = CCSprite::create(isColonThreeEnabled() ? "qolmodButtonOverlaycolonthree.png"_spr : "qolmodButtonOverlay.png"_spr);

    btn = CCSprite::create("qolmodButtonBG.png"_spr);
    btn->addChildAtPosition(btnOverlay, Anchor::Center);
    menu->addChild(btn);
    
    this->addChild(menu);
    this->setZOrder(69420 - 1);
    this->scheduleUpdate();

    UpdateVisible(true);

    return true;
}

void AndroidBall::onOpenMenu()
{
    if (Client::get()->useImGuiUI())
    {
        Client::get()->toggleWindowVisibility(WindowTransitionType::Vertical);
    }
    else
    {
        AndroidUI::addToScene();
    }
}

bool AndroidBall::_ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (!CCLayer::ccTouchBegan(touch, event))
        return false;

    if (!this->isVisible())
        return false;

    doingThing = false;
    dragging = false;

    auto point = btn->boundingBox();
    point.origin += ccp(btn->getContentWidth() / 2, btn->getContentHeight() / 2);

    if (point.containsPoint(btn->convertToNodeSpace(touch->getLocation())))
    {
        auto scale = UnspeedhackedAction::create(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2));
        scale->setTag(69);

        btn->runAction(scale);
        doingThing = true;
    }

    return doingThing;
}

bool AndroidBall::_ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return false;
    #endif

    if (doingThing)
    {
        if (!dragging)
            onOpenMenu();

        auto scale = UnspeedhackedAction::create(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1)));
        scale->setTag(69);

        btn->runAction(scale);

        doingThing = false;

        if (Module::get("save-pos")->enabled)
        {
            Mod::get()->setSavedValue("posX", position.x);
            Mod::get()->setSavedValue("posY", position.y);
        }
    }

    return doingThing;
}

bool AndroidBall::_ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return false;
    #endif

    if (doingThing && !btn->getActionByTag(69))
    {
        if (btn->getPosition().getDistance(touch->getLocation()) > 7.5f)
        {
            dragging = true;
        }

        if (!canDrag->enabled)
            dragging = false;

        if (dragging)
        {
            position = touch->getLocation();
            menu->setPosition(position);
        }
    }

    CCLayer::ccTouchMoved(touch, event);
    return doingThing;
}

void AndroidBall::update(float dt)
{
    UpdateVisible(false);
}

#define TYPE_CHECK(__type__) \
if (getParent()->getChildByType<__type__>(0)) \
    return true

bool AndroidBall::editorShouldBeVisible()
{
    if (LevelEditorLayer::get()->getChildByType<EditorPauseLayer>(0))
        return true;

    TYPE_CHECK(SetupCameraModePopup);
    TYPE_CHECK(LevelSettingsLayer);
    TYPE_CHECK(EditGameObjectPopup);
    TYPE_CHECK(CustomizeObjectLayer);
    TYPE_CHECK(SetGroupIDLayer);
    TYPE_CHECK(SetupTriggerPopup);

    return false;
}

void AndroidBall::UpdateVisible(bool i)
{
    bool vis = true;

    if (Mod::get()->getSavedValue<bool>("disable-gp_enabled") && this->getParent())
        vis = !(this->getParent()->getChildByType<PlayLayer>(0) && !this->getParent()->getChildByType<PauseLayer>(0));

    if (Mod::get()->getSavedValue<bool>("disable-editor_enabled") && this->getParent() && this->getParent()->getChildByType<LevelEditorLayer>(0))
        vis = editorShouldBeVisible();

    #ifdef GEODE_IS_DESKTOP
    if (vis)
        vis = !mod->enabled;
    #endif

    this->setVisible(vis);

    menu->setScale(clampf(Mod::get()->getSavedValue<float>("button-scale", 1), 0.2f, 1));

    menu->setPosition(position);

    ColourUtility::pastel++;

    btnOverlay->setColor(ColourUtility::getPastelColour());
    instance = this;

    if (btn->getActionByTag(69))
        return;

    if (i)
    {
        if (PlayLayer::get())
        {
            btn->setOpacity(50);
            btnOverlay->setOpacity(50);

            i = false;
        }
    }

    int op = Mod::get()->getSavedValue<int>("normal-opacity", 255);

    if (PlayLayer::get())
    {
        if (CCScene::get()->getChildByType<PauseLayer>(0))
        {
            op = Mod::get()->getSavedValue<int>("normal-opacity", 255);
        }
        else
        {
            op = Mod::get()->getSavedValue<int>("gameplay-opacity", 50);
        }
    }
    else if (GameManager::sharedState()->m_levelEditorLayer)
    {
        if (CCScene::get()->getChildByType<EditorPauseLayer>(0))
        {
            op = Mod::get()->getSavedValue<int>("normal-opacity", 255);
        }
        else
        {
            op = Mod::get()->getSavedValue<int>("editor-opacity", 50);
        }
    }

    if (op != btn->getOpacity())
    {
        if (i)
        {
            btn->setOpacity(op);
            btnOverlay->setOpacity(op);
        }
        else
        {
            auto action = UnspeedhackedAction::create(CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2));
            action->setTag(69);

            auto action2 = UnspeedhackedAction::create(CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2));
            action2->setTag(69);

            btn->runAction(action);
            btnOverlay->runAction(action2);
        }
    }
}

AndroidBall::~AndroidBall()
{
    if (instance == this)
        instance = nullptr;
}

float AndroidBall::clampf(float v, float min, float max)
{
    if (v < min)
        v = min;

    if (v > max)
        v = max;

    return v;
}

bool AndroidBall::isColonThreeEnabled()
{
    return Mod::get()->getSavedValue<bool>("colon-three-secwet-uwu-:3", false);
}

void AndroidBall::setColonThreeEnabled()
{
    Mod::get()->setSavedValue<bool>("colon-three-secwet-uwu-:3", !isColonThreeEnabled());

    auto spr = CCSprite::create(isColonThreeEnabled() ? "qolmodButtonOverlaycolonthree.png"_spr : "qolmodButtonOverlay.png"_spr)->getTexture();
    btnOverlay->setTexture(spr);
    
#ifndef GEODE_IS_IOS
    auto over = CCClippingNode::create(btn);
    over->setAlphaThreshold(0.9f);
    
    auto inner = CCLayerColor::create(ccc4(255, 255, 255, 255));
    inner->setAnchorPoint(ccp(0.5f, 0.5f));
    inner->setContentSize(ccp(100, 100));
    inner->ignoreAnchorPointForPosition(false);
    inner->runAction(CCFadeOut::create(1));

    over->addChild(inner);

    menu->addChild(over);
#endif
}

class $modify (CCScene)
{
    int getHighestChildZ()
    {
        if (!AndroidBall::get())
            return CCScene::getHighestChildZ();

        auto ins = AndroidBall::get();

        auto v = ins->getZOrder();
        ins->setZOrder(-1);

        int value = CCScene::getHighestChildZ();
        ins->setZOrder(v);

        return value;
    }
};

#ifdef __APPLE__
class $modify (AchievementNotifier)
#else
class $modify (AppDelegate)
#endif
{
    void willSwitchToScene(CCScene* newScene)
    {
        #ifdef __APPLE__
        AchievementNotifier::willSwitchToScene(newScene);
        #else
        AppDelegate::willSwitchToScene(newScene);
        #endif

        if (!newScene)
            return;

        if (newScene->getChildByType<LoadingLayer>(0))
            return; // fixes texture ldr

        if (auto ball = newScene->getChildByType<AndroidBall>(0))
        {
            AndroidBall::instance = ball;

            return;
        }

        auto ball = AndroidBall::create();

        newScene->addChild(ball);
    }
};

void QOLModTouchDispatcher::touches(CCSet* touches, CCEvent* event, unsigned int type)
{
    bool sendToGame = true;

    if (AndroidUI::get())
        return CCTouchDispatcher::touches(touches, event, type);

    if (touches)
    {
        if (auto ball = AndroidBall::get())
        {
            if (auto t = as<CCTouch*>(touches->anyObject()))
            {
                if (type == ccTouchType::CCTOUCHBEGAN)
                    sendToGame = !ball->_ccTouchBegan(t, event);

                if (type == ccTouchType::CCTOUCHMOVED)
                    sendToGame = !ball->_ccTouchMoved(t, event);

                if (type == ccTouchType::CCTOUCHENDED)
                    sendToGame = !ball->_ccTouchEnded(t, event);
            }
        }
    }

    if (sendToGame)
        CCTouchDispatcher::touches(touches, event, type);
}