#include "FloatingUIButton.hpp"
#include "FloatingUIManager.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "../../Utils/RealtimeAction.hpp"
#include "../Modules/SmoothMoveButton.hpp"

using namespace geode::prelude;

#define BUTTON_RADIUS 40
#define ICON_SIZE 22.5f

FloatingUIButton* FloatingUIButton::create(std::function<void()> onClick)
{
    auto pRet = new FloatingUIButton();

    pRet->setOnClick(onClick);

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool FloatingUIButton::init()
{
    FloatingUIManager::get()->addButton(this);

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentSize(ccp(BUTTON_RADIUS, BUTTON_RADIUS));
    this->ignoreAnchorPointForPosition(false);
    this->scheduleUpdate();
    this->onEnter();

    updateSprites();
    return true;
}

bool FloatingUIButtonVisibility::shouldShow()
{
    if (auto pl = PlayLayer::get())
    {
        if (pl->m_isPaused && showInPauseMenu)
            return true;

        if (!pl->m_isPaused && showInGame)
            return true;
    }
    else if (auto ed = LevelEditorLayer::get())
    {
        if (pl->m_isPaused && showInEditorPauseMenu)
            return true;

        if (!pl->m_isPaused && showInEditor)
            return true;
    }
    else
    {
        if (showInMenu)
            return true;
    }

    return false;
}

void FloatingUIButton::updateSprites(std::string background, std::string overlay, bool backgroundSpriteSheet, bool overlaySpriteSheet)
{
    this->background = background;
    this->overlay = overlay;
    this->backgroundUsesSpriteSheet = backgroundSpriteSheet;
    this->overlayUsesSpriteSheet = overlaySpriteSheet;

    updateSprites();
}

void FloatingUIButton::updateSprites()
{
    this->removeAllChildren();

    if (!background.empty())
    {
        CCSprite* bg = nullptr;

        if (backgroundUsesSpriteSheet)
            bg = CCSprite::createWithSpriteFrameName(background.c_str());
        else
            bg = CCSprite::create(background.c_str());
        
        if (bg)
        {
            bg->setPosition(getContentSize() / 2);
            bg->setScale(scale);
            this->addChild(bg);
        }
    }

    if (!overlay.empty())
    {
        CCSprite* ov = nullptr;

        if (overlayUsesSpriteSheet)
            ov = CCSprite::createWithSpriteFrameName(overlay.c_str());
        else
            ov = CCSprite::create(overlay.c_str());

        if (ov)
        {
            ov->setPosition(getContentSize() / 2);
            ov->setScale((ICON_SIZE / std::max<float>(ov->getContentWidth(), ov->getContentHeight())) * scale);
            this->addChild(ov);
        }
    }
}

void FloatingUIButton::update(float dt)
{
    auto v = visibilityConf.shouldShow();

    this->setVisible(v);

    if (!v)
        return;

    dt = Speedhack::get()->getRealDeltaTime();
    float t = 10 * dt;

    this->setPosition(ccp(
        std::lerp<double>(getPositionX(), position.x, t),
        std::lerp<double>(getPositionY(), position.y, t)
    ));

    _opacity = std::lerp<double>(_opacity, isSelected ? 1.0f : opacity, t);

    for (auto child : CCArrayExt<CCSprite*>(getChildren()))
    {
        child->setOpacity(255 * _opacity);
    }
}

FloatingUIButton::~FloatingUIButton()
{
    FloatingUIManager::get()->removeButton(this);
}

void FloatingUIButton::setOnClick(std::function<void()> onClick)
{
    this->onClick = onClick;
}

void FloatingUIButton::setButtonVisibilityConfig(FloatingUIButtonVisibility conf)
{
    this->visibilityConf = conf;
}

void FloatingUIButton::setMovable(bool movable)
{
    this->movable = movable;
}

void FloatingUIButton::updatePosition(cocos2d::CCPoint point)
{
    auto safe = utils::getSafeAreaRect();
    point.x = std::max<float>(safe.getMinX(), point.x);
    point.x = std::min<float>(safe.getMaxX(), point.x);
    point.y = std::max<float>(safe.getMinY(), point.y);
    point.y = std::min<float>(safe.getMaxY(), point.y);

    this->position = point;

    if (!SmoothMoveButton::get()->getRealEnabled())
        setPosition(position);
}

bool FloatingUIButton::ccTouchBegan(CCTouch* touch)
{
    if (!visibilityConf.shouldShow())
        return false;

    if (cocos2d::ccpDistance(position, touch->getLocation()) < (BUTTON_RADIUS / 2) * scale)
    {
        setZOrder(FloatingUIManager::get()->getHighestButtonZ() + 1);

        this->stopAllActions();
        this->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.9f), 2)));

        isMoving = false;
        isSelected = true;
        return true;
    }

    return false;
}

void FloatingUIButton::ccTouchMoved(CCTouch* touch)
{
    if (movable)
    {
        if (cocos2d::ccpDistance(touch->getStartLocation(), touch->getLocation()) > 5)
        {
            isMoving = true;
        }
    }

    if (isMoving)
    {
        updatePosition(touch->getLocation());
    }
}

void FloatingUIButton::ccTouchEnded(CCTouch* touch)
{
    if (!isMoving)
    {
        if (onClick)
            onClick();
    }

    this->stopAllActions();
    this->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1))));
    isSelected = false;
}