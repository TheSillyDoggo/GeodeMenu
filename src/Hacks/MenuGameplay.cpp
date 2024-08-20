#include <Geode/Geode.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class MenuGameDelegate : public CCLayer
{
    public:
        static inline MenuGameDelegate* instance = nullptr;

        bool init()
        {
            if (!CCLayer::init())
                return false;

            instance = this;

            this->setKeyboardEnabled(true);
            this->setTouchEnabled(true);

            CCTouchDispatcher::get()->addTargetedDelegate(this, -69, true);

            return true;
        }

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
        {
            if (auto mgl = getGameLayer())
            {    
                if (Client::GetModuleEnabled("main-menu-gameplay") && (pTouch ? mgl->ccTouchBegan(pTouch, pEvent) : true))
                {
                    if (mgl->m_playerObject && !mgl->m_playerObject->m_isSpider)
                        mgl->m_playerObject->pushButton(PlayerButton::Jump);

                    return true;
                }
            }

            return false;
        }

        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
        {
            if (Client::GetModuleEnabled("main-menu-gameplay"))
            {
                if (auto mgl = getGameLayer())
                {
                    if (mgl->m_playerObject && !mgl->m_playerObject->m_isSpider)
                        mgl->m_playerObject->releaseButton(PlayerButton::Jump);
                }
            }
        }

        MenuGameLayer* getGameLayer()
        {
            if (!getParent())
            {
                return nullptr;
            }

            return as<MenuGameLayer*>(getParent());
        }

        ~MenuGameDelegate()
        {
            instance = nullptr;
        }

        CREATE_FUNC(MenuGameDelegate);
};

class $modify (MenuGameLayer)
{   
    void tryJump(float p0)
    {
        if (!Client::GetModuleEnabled("main-menu-gameplay"))
            MenuGameLayer::tryJump(p0);
    }

    virtual bool init()
    {
        if (!MenuGameLayer::init())
            return false;

        this->setTouchEnabled(true);
        auto gp = MenuGameDelegate::create();

        this->addChild(gp);
        return true;
    }
};

#ifndef GEODE_IS_IOS

class $modify (CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
    {
        if (!isKeyRepeat && MenuGameDelegate::instance)
        {
            if (key == as<enumKeyCodes>(38) || key == enumKeyCodes::KEY_W)
            {
                if (isKeyDown)
                    MenuGameDelegate::instance->ccTouchBegan(nullptr, nullptr);
                else
                    MenuGameDelegate::instance->ccTouchEnded(nullptr, nullptr);
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }

    static void onModify(auto& self) {
        auto hook = self.getHook("CCKeyboardDispatcher::dispatchKeyboardMSG");

        Loader::get()->queueInMainThread([hook]
        {
            auto modu = Client::GetModule("main-menu-gameplay");
            modu->addHookRaw(hook);
        });
    }
};

#endif