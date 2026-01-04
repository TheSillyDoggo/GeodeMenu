#include "AndroidBall.hpp"
#include "AndroidUI.hpp"
#include "Modules/SavePosition.hpp"
#include "Modules/HideButton.hpp"
#include "Modules/AllowDragging.hpp"
#include "Modules/SmoothMoveButton.hpp"
#include "Modules/ButtonScale.hpp"
#include "Modules/UseColonThreeButton.hpp"
#include "../Utils/ColourUtils.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"
#include "../Utils/RealtimeAction.hpp"

AndroidBall* AndroidBall::get()
{
    static Ref<AndroidBall> instance = nullptr;

    if (!instance)
        instance = create();

    return instance;
}

bool AndroidBall::init()
{
    this->retain();
    this->onEnter();
    this->scheduleUpdate();

    background = CCSprite::create("circle.png");
    overlay = CCSprite::create();

    this->setOpacity(normalOpacity * 255);

    if (SavePosition::get()->getUserEnabled())
    {
        this->setPosition(Mod::get()->getSavedValue("posX", 32), Mod::get()->getSavedValue("posY", CCDirector::get()->getWinSize().height / 2));
        position = getPosition();
    }
    else
    {
        this->setPosition(ccp(32, CCDirector::get()->getWinSize().height / 2));
        position = getPosition();
    }

    setColonThreeSecret(UseColonThreeButton::get()->getRealEnabled());
    reloadTextures();

    this->addChild(background);
    this->addChild(overlay);
    return true;
}

void AndroidBall::setColonThreeSecret(bool enabled)
{ 
    if (auto spr = CCSprite::create(enabled ? "qolmodButtonOverlaycolonthree.png"_spr : "qolmodButtonOverlay.png"_spr))
    {
        overlay->setTexture(spr->getTexture());
        colonThreeEnabled = enabled;
    }
    else
    {
        overlay->setTexture(CCSprite::create()->getTexture());
        missingImportantAssets = true;
    }

    CCRect rect = CCRectZero;
    rect.size = overlay->getTexture()->getContentSize();

    overlay->setTextureRect(rect);
}

bool AndroidBall::getColonThreeSecret()
{
    return colonThreeEnabled;
}

void AndroidBall::setButtonScale(float scale)
{
    scale = std::clamp<float>(scale, 0.6f, 1.0f);
    this->scale = scale;

    background->setScale(scale);
    overlay->setScale(scale);
}

void AndroidBall::setSmoothMove(bool smooth)
{
    this->smoothMove = smooth;
}

void AndroidBall::reloadTextures()
{
    missingImportantAssets = false;

    auto spr = CCSprite::create("qolmodButtonBG.png"_spr);

    if (!spr)
    {
        spr = CCSprite::create("circle.png");
        missingImportantAssets = true;
    }

    background->setTexture(spr->getTexture());

    CCRect rect = CCRectZero;
    rect.size = background->getTexture()->getContentSize();
    background->setTextureRect(rect);

    setColonThreeSecret(colonThreeEnabled);
}

bool AndroidBall::shouldFunction()
{
    #ifdef GEODE_IS_DESKTOP

    if (HideButton::get()->getUserEnabled())
        return false;
    
    #endif

    if (CCScene::get() && CCScene::get()->getChildByType<LoadingLayer>(0))
        return false;

    if (PlayLayer::get() && (!PlayLayer::get()->m_levelEndAnimationStarted && !PlayLayer::get()->m_isPaused))
        return false;

    return true;
}

void AndroidBall::update(float dt)
{
    if (!shouldFunction())
        return;

    setSmoothMove(SmoothMoveButton::get()->getRealEnabled());
    setButtonScale(ButtonScale::get()->getValue());

    if (getColonThreeSecret() != UseColonThreeButton::get()->getRealEnabled())
        setColonThreeSecret(UseColonThreeButton::get()->getRealEnabled());

    if (smoothMove)
    {
        float t = 10 * Speedhack::get()->getRealDeltaTime();

        this->setPosition(ccp(
            std::lerp<double>(getPositionX(), position.x, t),
            std::lerp<double>(getPositionY(), position.y, t)
        ));
    }
    else
    {
        this->setPosition(position);
    }

    auto winSize = CCDirector::get()->getWinSize();

    this->setPosition(ccp(
        std::clamp<float>(getPositionX(), 0, winSize.width),
        std::clamp<float>(getPositionY(), 0, winSize.height)
    ));

    overlay->setColor(ColourUtils::get()->getPastel("ui-button"));

    if (!AndroidUI::get() && !getActionByTag(69) && getOpacity() != (normalOpacity * 255) && !isDragging)
    {
        auto fadeBack = RealtimeAction::create(CCFadeTo::create(0.25f, normalOpacity * 255));
        fadeBack->setTag(69);
        this->runAction(fadeBack);
    }
}

void AndroidBall::onOpenMenu()
{
    AndroidUI::addToScene();
}

void AndroidBall::visit()
{
    if (!shouldFunction())
        return;

    CCNode::visit();
}

void AndroidBall::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);

    background->setOpacity(opacity);
    overlay->setOpacity(opacity);
}

bool AndroidBall::ccTouchBegan(CCTouch* touch)
{
    CCRect rect = { getPositionX() - 25.0f * scale, getPositionY() - 25.0f * scale, 50 * scale, 50 * scale };

    if (rect.containsPoint(touch->getLocation()))
    {
        isDragging = true;

        this->stopAllActions();
        this->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.9f), 2)));
        auto fadeIn = RealtimeAction::create(CCFadeTo::create(0.25f, 255));
        fadeIn->setTag(69);
        this->runAction(fadeIn);

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

        if (!isMoving && touch->getLocation().getDistance(touch->getStartLocation()) > dis && AllowDragging::get()->getRealEnabled())
        {
            isMoving = true;
            moveOffset = touch->getLocation() - getPosition();
        }

        if (isMoving)
        {
            //position = touch->getLocation() - moveOffset;
            position = touch->getLocation();

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
    else
    {
        auto fadeBack = RealtimeAction::create(CCFadeTo::create(0.25f, normalOpacity * 255));
        fadeBack->setTag(69);
        this->runAction(fadeBack);
    }

    this->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1))));
    Mod::get()->setSavedValue("posX", position.x);
    Mod::get()->setSavedValue("posY", position.y);

    isDragging = false;
    isMoving = false;

    return false;
}

bool AndroidBall::areImportantTexturesMissing()
{
    return missingImportantAssets;
}