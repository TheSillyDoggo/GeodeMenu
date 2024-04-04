#include "AndroidBall.h"


void AndroidBall::onOpenMenu()
{
    AndroidUI::addToScene();
}

bool AndroidBall::init()
{
    if (!CCLayer::init())
        return false;

    this->setID("android-ball");

    highest++;

    mod = Client::GetModule("hide-btn");
    mod2 = Client::GetModule("instant-fade");
    canDrag = Client::GetModule("allow-dragging");

    this->setTag(highest);
    instance = this;

    this->setTouchEnabled(true);
    this->setMouseEnabled(true);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -512 - 1, true);

    menu = CCMenu::create();
    menu->setPosition(ccp(0, 0));
    menu->setContentSize(ccp(0, 0));

    l = CCLabelBMFont::create(">_", "bigFont.fnt");
    l->setAnchorPoint(ccp(0.5f, 0.35f));

    btn = CircleButtonSprite::create(l, CircleBaseColor::Gray);
    menu->addChild(btn);
    this->addChild(menu);
    this->setZOrder(69420 - 1);
    this->scheduleUpdate();

    menu->setPosition(position);

    UpdateVisible(true);

    return true;
}

bool AndroidBall::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return false;
    #endif


    auto space = btn->convertTouchToNodeSpace(touch);

    doingThing = false;
    dragging = false;

    if (space.x > 0 && space.y > 0)
    {
        if (space.x < btn->getContentSize().width && space.y < btn->getContentSize().height)
        {
            btn->runAction(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2));
            doingThing = true;
        }
    }

    return doingThing;
}

void AndroidBall::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    #ifdef GEODE_IS_DESKTOP
    if (mod->enabled)
        return;
    #endif

    if (doingThing)
    {
        if (!dragging)
            onOpenMenu();

        btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1)));

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
        return;
    #endif

    if (doingThing && (btn->numberOfRunningActions() == 0))
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
}

void AndroidBall::update(float dt)
{
    UpdateVisible(false);
}

void AndroidBall::UpdateVisible(bool i)
{
    #ifdef GEODE_IS_DESKTOP
    this->setVisible(!mod->enabled);
    #endif

    menu->setScale(clampf(Mod::get()->getSavedValue<float>("button-scale", 1), 0.2f, 1));

    menu->setPosition(position);

    ColourUtility::pastel++;

    l->setColor(ColourUtility::getPastelColour(ColourUtility::pastel));
    instance = this;

    if (btn->numberOfRunningActions() != 0)
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
        if (CCDirector::get()->getRunningScene()->getChildByID("editor-pause-layer"))
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
            btn->runAction(CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2));
            l->runAction(CCEaseInOut::create(CCFadeTo::create(0.35f * (mod2->enabled ? 0 : 1), op), 2));
        }
    }
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
    void willSwitchToScene(CCScene* p0)
    {
        AppDelegate::willSwitchToScene(p0);

        if (p0 == nullptr)
            return; // something real bad happened, gd will probably shit itself :(

        if (getChildOfType<LoadingLayer>(p0, 0))
            return; // fix texture ldr

        for (size_t i = 0; i < p0->getChildrenCount(); i++)
        {
            auto id = as<CCLayer*>(p0->getChildren()->objectAtIndex(i))->getID();
            
            if (id == "android-ball")
            {
                CCTouchDispatcher::get()->addTargetedDelegate(as<AndroidBall*>(p0->getChildren()->objectAtIndex(i)), -514, true);

                return;
            }
        }

        p0->addChild(AndroidBall::create());
        cocos::handleTouchPriority(AndroidBall::instance);
    }
};