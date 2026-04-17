#include "Button.hpp"
#include <RealtimeAction.hpp>
#include <Speedhack.hpp>

using namespace geode::prelude;
using namespace qolmod;

Button* Button::create(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector)
{
    return create(normalSprite, nullptr, nullptr, target, selector);
}

Button* Button::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
    return create(normalSprite, selectedSprite, nullptr, target, selector);
}

Button* Button::create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
    auto pRet = new Button();

    if (pRet && pRet->init(normalSprite, selectedSprite, disabledSprite, target, selector))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Button::init(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
    if (!CCMenuItemSprite::initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
        return false;

    this->setContentSize(normalSprite->getScaledContentSize());

    normalSprite->setPosition(getContentSize() / 2);
    normalSprite->setAnchorPoint(ccp(0.5f, 0.5f));

    scheduleUpdate();
    return true;
}

void Button::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();

    if (!m_bEnabled || !m_bSelected)
    {
        t = 0;
        v = 0;
        d = 0;
        return;
    }

    if (repeatEnabled)
    {
        float maxSpeed = 0.08f;
        float repTimeInc = 0.003f;
        float repeatTime = 0.15f;
        float startDelay = 0.4f;

        t += dt;

        if (t > startDelay)
        {
            v += dt;

            if (v > repeatTime - d)
            {
                v -= repeatTime - d;

                d += repTimeInc;

                if (d > repeatTime - maxSpeed)
                    d = repeatTime - maxSpeed;

                CCMenuItemSprite::activate();

                auto sequence = CCSequence::create(
                    CCEaseBackOut::create(CCScaleTo::create(0.07f, 1.2f)),
                    CCDelayTime::create(0.02f),
                    CCScaleTo::create(0.1f, 1.0f),
                    nullptr
                );

                this->runAction(RealtimeAction::create(sequence));
            }
        }
    }
}

void Button::setRepeatEnabled(bool en)
{
    this->repeatEnabled = en;
}

bool Button::isRepeatEnabled()
{
    return repeatEnabled;
}

void Button::selected()
{
    this->stopAllActions();
    this->runAction(
        RealtimeAction::create(
            CCEaseBounceOut::create(CCScaleTo::create(0.3f, m_scaleMultiplier))
        )
    );

    CCMenuItemSprite::selected();
}

void Button::unselected()
{
    this->stopAllActions();
    this->runAction(
        RealtimeAction::create(
            CCEaseBounceOut::create(CCScaleTo::create(0.4f, 1))
        )
    );

    CCMenuItemSprite::unselected();
}

void Button::activate()
{
    this->stopAllActions();
    this->setScale(1);
    CCMenuItemSprite::activate();
}

void Button::draw()
{
    return;

    ccDrawColor4F(1, 1, 0, 1);
    glLineWidth(3);
    ccDrawRect(ccp(0, 0), getContentSize());
}