#include "AndroidBall.h"

bool AndroidBall::init()
{
    if (!CCLayer::init())
        return false;

    this->setID("android-ball");
    this->setMouseEnabled(false);
    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -512 - 1, true);

    highest++;
    this->setTag(highest);
    instance = this;

    mod = Client::GetModule("hide-btn");
    mod2 = Client::GetModule("instant-fade");
    canDrag = Client::GetModule("allow-dragging");

    menu = CCMenu::create();
    menu->setPosition(position);
    menu->setContentSize(ccp(0, 0));

    l = CCLabelBMFont::create(">_", "bigFont.fnt");
    l->setAnchorPoint(ccp(0.5f, 0.35f));

    btn = CircleButtonSprite::create(l, CircleBaseColor::Gray);
    menu->addChild(btn);
    
    this->addChild(menu);
    this->setZOrder(69420 - 1);
    this->scheduleUpdate();

    UpdateVisible(true);

    return true;
}

void AndroidBall::onOpenMenu()
{
    AndroidUI::addToScene();
}

bool AndroidBall::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (!CCLayer::ccTouchBegan(touch, event))
        return false;

    if (!this->isVisible())
        return false;

    doingThing = false;
    dragging = false;

    auto point = btn->boundingBox();
    point.origin += ccp(btn->getContentWidth() / 2, btn->getContentHeight() / 2);

    if (point.containsPoint(btn->convertTouchToNodeSpace(touch)))
    {
        auto scale = CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2);
        scale->setTag(69);

        btn->runAction(scale);
        doingThing = true;
    }

    return doingThing;
}

void AndroidBall::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return CCLayer::ccTouchEnded(touch, event);
    #endif

    if (doingThing)
    {
        if (!dragging)
            onOpenMenu();

        auto scale = CCEaseBackOut::create(CCScaleTo::create(0.35f, 1));
        scale->setTag(69);

        btn->runAction(scale);

        doingThing = false;

        if (Client::GetModuleEnabled("save-pos"))
        {
            Mod::get()->setSavedValue("posX", position.x);
            Mod::get()->setSavedValue("posY", position.y);
        }
    }
}

void AndroidBall::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return CCLayer::ccTouchMoved(touch, event);
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
}

void AndroidBall::update(float dt)
{
    UpdateVisible(false);
}

void AndroidBall::UpdateVisible(bool i)
{
    bool vis = true;

    if (Client::GetModuleEnabled("disable-gp") && this->getParent())
        vis = !(getChildOfType<PlayLayer>(this->getParent(), 0) && !getChildOfType<PauseLayer>(this->getParent(), 0));

    #ifdef GEODE_IS_DESKTOP
    if (vis)
        vis = !mod->enabled;
    #endif

    this->setVisible(vis);

    menu->setScale(clampf(Mod::get()->getSavedValue<float>("button-scale", 1), 0.2f, 1));

    menu->setPosition(position);

    ColourUtility::pastel++;

    l->setColor(ColourUtility::getPastelColour(ColourUtility::pastel));
    instance = this;

    if (btn->getActionByTag(69))
        return;

    if (i)
    {
        if (PlayLayer::get())
        {
            btn->setOpacity(50);
            l->setOpacity(50);

            i = false;
        }
    }

    int op = Mod::get()->getSavedValue<int>("normal-opacity", 255);

    if (PlayLayer::get())
    {
        if (getChildOfType<PauseLayer>(CCScene::get(), 0))
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
        if (getChildOfType<EditorPauseLayer>(CCScene::get(), 0))
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
            l->setOpacity(op);
        }
        else
        {
            auto action = CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2);
            action->setTag(69);

            auto action2 = CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2);
            action2->setTag(69);

            btn->runAction(action);
            l->runAction(action2);
        }
    }
}

AndroidBall::~AndroidBall()
{
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

class $modify (CCScene)
{
    int getHighestChildZ()
    {
        if (!getChildOfType<AndroidBall>(CCScene::get(), 0))
            return CCScene::getHighestChildZ();

        auto ins = getChildOfType<AndroidBall>(CCScene::get(), 0);

        auto v = ins->getZOrder();
        ins->setZOrder(-1);

        int value = CCScene::getHighestChildZ();
        ins->setZOrder(v);

        return value;
    }
};

class $modify (AppDelegate)
{
    void willSwitchToScene(CCScene* newScene)
    {
        AppDelegate::willSwitchToScene(newScene);

        if (newScene == nullptr)
            return; // something real bad happened, gd will probably shit itself :(

        if (getChildOfType<LoadingLayer>(newScene, 0))
            return; // fix texture ldr

        if (auto ball = getChildOfType<AndroidBall>(newScene, 0))
            ball->removeFromParent();

        newScene->addChild(AndroidBall::create());
        cocos::handleTouchPriority(AndroidBall::instance);

        if (auto shop = getChildOfType<GJShopLayer>(newScene, 0))
        {
            cocos::handleTouchPriority(shop);
        }
    }
};