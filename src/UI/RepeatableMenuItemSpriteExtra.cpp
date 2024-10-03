#include "RepeatableMenuItemSpriteExtra.hpp"

void RepeatableMenuItemSpriteExtra::update(float dt)
{
    if (!m_bEnabled)
    {
        m_bSelected = false;
    }

    if (!m_bSelected)
    {
        t = 0;
        v = 0;
        d = 0;
        return;
    }

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

            activate();

            auto sequence = CCSequence::create(CCEaseBackOut::create(CCScaleBy::create(0.07f, 1.2f)), CCDelayTime::create(0.02f), CCScaleBy::create(0.1f, 1.0f / 1.2f), nullptr);

            this->runAction(sequence);
        }
    }
}

RepeatableMenuItemSpriteExtra* RepeatableMenuItemSpriteExtra::create(cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback)
{
    auto pRet = new RepeatableMenuItemSpriteExtra();

    if (pRet && pRet->init(sprite, disabledSprite, target, callback))
    {
        pRet->scheduleUpdate();

        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

RepeatableMenuItemSpriteExtra* RepeatableMenuItemSpriteExtra::create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback)
{
    return create(sprite, nullptr, target, callback);
}