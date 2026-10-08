#include "FloatingButtonBase.hpp"
#include "FloatingUIManager.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "../../Utils/RealtimeAction.hpp"
#include "../Modules/SmoothMoveButton.hpp"

using namespace geode::prelude;

#define BUTTON_RADIUS 40.0f

FloatingButtonBase* FloatingButtonBase::create()
{
    auto pRet = new FloatingButtonBase();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool FloatingButtonBase::init()
{
    FloatingUIManager::get()->addButton(this);

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentSize(ccp(BUTTON_RADIUS, BUTTON_RADIUS));
    this->ignoreAnchorPointForPosition(false);
    this->scheduleUpdate();
    this->onEnter();

    return true;
}

void FloatingButtonBase::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    float t = 10 * dt;

    this->CCNode::setPosition(ccp(
        std::lerp<double>(getPositionX(), position.x, t),
        std::lerp<double>(getPositionY(), position.y, t)
    ));

    _opacity = std::lerp<double>(_opacity, isSelected ? 1.0f : opacity, t);
    updateVisuals(_opacity);
}

void FloatingButtonBase::animate(bool release, bool clicked)
{
    this->stopAllActions();

    switch (animation)
    {
        case AnimationType::Shrink:
            if (release)
                this->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1))));
            else
                this->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.9f), 2)));
            return;

        case AnimationType::GD:
            if (!release)
                this->runAction(RealtimeAction::create(CCEaseBounceOut::create(CCScaleTo::create(0.3f, 1.26f))));
            else
            {
                if (clicked)
                {
                    this->stopAllActions();
                    this->setScale(1.0f);
                }
                else
                    this->runAction(RealtimeAction::create(CCEaseBounceOut::create(CCScaleTo::create(0.4f, 1.0f))));
            }
            return;
    }
}

void FloatingButtonBase::setMovable(bool movable)
{
    this->isMovable = movable;
}

void FloatingButtonBase::setBaseScale(float scale)
{
    this->scale = clampf(scale, 0.1f, 1.0f);
    
    // funny cocos feature people dont use :3
    this->m_sAdditionalTransform.tx = scale;
    this->m_sAdditionalTransform.ty = scale;
}

void FloatingButtonBase::setBaseOpacity(float opacity)
{
    this->opacity = _opacity = clampf(opacity, 0.1f, 1.0f);
}

void FloatingButtonBase::setAnimation(AnimationType anim)
{
    this->animation = anim;
}

void FloatingButtonBase::setPosition(cocos2d::CCPoint position, bool instant)
{
    auto safe = utils::getSafeAreaRect();
    position.x = std::max<float>(safe.getMinX(), position.x);
    position.x = std::min<float>(safe.getMaxX(), position.x);
    position.y = std::max<float>(safe.getMinY(), position.y);
    position.y = std::min<float>(safe.getMaxY(), position.y);

    this->position = position;

    if (!SmoothMoveButton::get()->getRealEnabled() || instant)
        CCNode::setPosition(position);
}

FloatingButtonBase::~FloatingButtonBase()
{
    FloatingUIManager::get()->removeButton(this);
}

// touch handling
bool FloatingButtonBase::doesTouchOverlap(cocos2d::CCPoint touchPoint)
{
    float radius = (BUTTON_RADIUS / 2) * scale;

    switch (hitboxType)
    {
        case HitboxType::Circle:
            return cocos2d::ccpDistance(position, touchPoint) < radius;

        case HitboxType::Square:
            return fabs(position.x - touchPoint.x) < radius && fabs(position.y - touchPoint.y) < radius;

        default:
            return false;
    }
}

bool FloatingButtonBase::ccTouchBegan(qolmod::Touch* touch)
{
    if (!isVisible())
        return false;

    if (doesTouchOverlap(touch->location))
    {
        setZOrder(FloatingUIManager::get()->getHighestButtonZ() + 1);

        animate(false);

        isMoving = false;
        isSelected = true;
        return true;
    }

    return false;
}

void FloatingButtonBase::ccTouchMoved(qolmod::Touch* touch)
{
    if (isMovable && !isMoving)
    {
        if (cocos2d::ccpDistance(touch->startLocation, touch->location) > 5)
        {
            isMoving = true;
        }
    }

    if (isMoving)
    {
        setPosition(touch->location, false);
    }
}

void FloatingButtonBase::ccTouchEnded(qolmod::Touch* touch)
{
    bool clicked = false;

    if (isMovable ? !isMoving : doesTouchOverlap(touch->location))
    {
        onClick();
        clicked = true;
    }

    animate(true, clicked);
    isSelected = false;
}

void FloatingButtonBase::draw()
{
    return;

    ccDrawColor4B(!isSelected ? ccc4(0, 255, 0, 255) : ccc4(0, 0, 255, 255));
    glLineWidth(3);

    switch (hitboxType)
    {
        case HitboxType::Circle:
            ccDrawCircle(getContentSize() / 2, BUTTON_RADIUS / 2 * scale, 360, 360, false);
            return;

        case HitboxType::Square:
            ccDrawRect(getContentSize() / 2 - ccp(BUTTON_RADIUS / 2, BUTTON_RADIUS / 2), getContentSize() / 2 + ccp(BUTTON_RADIUS / 2, BUTTON_RADIUS / 2));
            return;
    }
}

// virtuals: to be overwritten
void FloatingButtonBase::setupChildren() {}
void FloatingButtonBase::updateVisuals(float) {}
void FloatingButtonBase::onClick() {}