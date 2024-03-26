/*#include <Geode/Geode.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class MenuGameDelegate : public CCLayer
{
    public:
        MenuGameLayer* mgl = nullptr;

        bool init()
        {
            if (!CCLayer::init())
                return false;

            this->setKeyboardEnabled(true);
            this->setTouchEnabled(true);

            CCKeyboardDispatcher::get()->forceAddDelegate(this);
            CCTouchDispatcher::get()->addTargetedDelegate(this, -69, true);

            return true;
        }

        virtual void keyDown(enumKeyCodes key)
        {
            log::info("down");
        }

		virtual void keyUp(enumKeyCodes key)
        {
            log::info("up");
        }

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
        {
            if (mgl->m_playerObject && !mgl->m_playerObject->m_isSpider)
                mgl->m_playerObject->pushButton(PlayerButton::Jump);

            return true;
        }

        virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
        {
            if (mgl->m_playerObject && !mgl->m_playerObject->m_isSpider)
                mgl->m_playerObject->releaseButton(PlayerButton::Jump);
        }

        CREATE_FUNC(MenuGameDelegate);
};

class $modify (MenuGameLayer)
{   
    void tryJump(float p0)
    {
        log::info("penis");
    }

    virtual bool init()
    {
        if (!MenuGameLayer::init())
            return false;

        auto gp = MenuGameDelegate::create();
        gp->mgl = this;
        this->addChild(gp);

        return true;
    }
};*/