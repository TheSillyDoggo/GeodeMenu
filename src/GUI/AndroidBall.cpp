#include "AndroidBall.hpp"
#include "AndroidUI.hpp"
#include "Modules/SavePosition.hpp"
#include "Modules/HideButton.hpp"
#include "Modules/AllowDragging.hpp"

AndroidBall* AndroidBall::get()
{
    static AndroidBall* instance = nullptr;

    if (!instance)
        instance = create();

    return instance;
}

bool AndroidBall::init()
{
    background = CCSprite::create("qolmodButtonBG.png"_spr);
    overlay = CCSprite::create("qolmodButtonOverlay.png"_spr);

    if (SavePosition::get()->getUserEnabled())
        this->setPosition(Mod::get()->getSavedValue("posX", 32), Mod::get()->getSavedValue("posY", CCDirector::get()->getWinSize().height / 2));

    setColonThreeSecret(Mod::get()->getSavedValue<bool>("colon-three-secwet-uwu-:3", false));

    this->onEnter();
    this->scheduleUpdate();
    this->addChild(background);
    this->addChild(overlay);
    return true;
}

void AndroidBall::setColonThreeSecret(bool enabled)
{
    overlay->setTexture(CCSprite::create(enabled ? "qolmodButtonOverlaycolonthree.png"_spr : "qolmodButtonOverlay.png"_spr)->getTexture());
    colonThreeEnabled = enabled;
}

bool AndroidBall::getColonThreeSecret()
{
    return colonThreeEnabled;
}

void AndroidBall::reloadTextures()
{
    log::info("Reloading textures");

    background->setTexture(CCSprite::create("qolmodButtonBG.png"_spr)->getTexture());
    setColonThreeSecret(colonThreeEnabled);
}

bool AndroidBall::shouldFunction()
{
    if (HideButton::get()->getUserEnabled())
        return false;

    if (CCScene::get() && CCScene::get()->getChildByType<LoadingLayer>(0))
        return false;

    return true;
}

void AndroidBall::update(float dt)
{
    if (!shouldFunction())
        return;

    this->setPositionX(std::min<float>(std::max<float>(0, getPositionX()), CCDirector::get()->getWinSize().width));
    this->setPositionY(std::min<float>(std::max<float>(0, getPositionY()), CCDirector::get()->getWinSize().height));
}

void AndroidBall::visit()
{
    if (!shouldFunction())
        return;

    CCNode::visit();
}

bool AndroidBall::ccTouchBegan(CCTouch* touch)
{
    CCRect rect = { getPositionX() - 25.0f, getPositionY() - 25.0f, 50, 50 };

    if (rect.containsPoint(touch->getLocation()))
    {
        isDragging = true;

        this->stopAllActions();
        this->runAction(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2));

        return true;
    }
    
    return false;
}

bool AndroidBall::ccTouchMoved(CCTouch* touch)
{
    if (isDragging)
    {
        #ifdef GEODE_IS_DESKTOP
        float dis = 1;
        #else
        float dis = 7.5f;
        #endif

        if (!isMoving && touch->getLocation().getDistance(touch->getStartLocation()) > dis)
        {
            isMoving = true;
            moveOffset = touch->getLocation() - getPosition();
        }

        if (isMoving)
        {
            setPosition(touch->getLocation() - moveOffset);

            return true;
        }
    }

    return false;
}

bool AndroidBall::ccTouchEnded(CCTouch* touch)
{
    if (isDragging && !isMoving)
    {
        onOpenMenu();
    }

    this->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1)));
    Mod::get()->setSavedValue("posX", getPositionX());
    Mod::get()->setSavedValue("posY", getPositionY());

    isDragging = false;
    isMoving = false;

    return false;
}

void AndroidBall::onOpenMenu()
{
    AndroidUI::addToScene();
}