#pragma once

#include <Geode/Geode.hpp>

class PopupBase : public cocos2d::CCLayerColor
{
    protected:
        cocos2d::CCSize m_size;
        bool m_noElasticity = false;
        cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
        cocos2d::CCMenu* m_buttonMenu = nullptr;
        cocos2d::CCLayerColor* blurLayer = nullptr;

        // pre 2.2081 compatibility
        virtual void keyDown(cocos2d::enumKeyCodes code);
        virtual void keyDown(cocos2d::enumKeyCodes code, double timestamp);

        virtual void keyBackClicked();
        virtual void registerWithTouchDispatcher(void);
        ~PopupBase();

        float calcPopupScale();

    public:
        bool addToScene = true;
        bool shouldVisit = true;
        cocos2d::CCLayer* m_mainLayer = nullptr;

        virtual bool init(float width, float height);
        virtual bool initAnchored(float width, float height);
        virtual bool setup();
        virtual void show();
        virtual void visit();

        void onClose(CCObject* sender);
        void onBtn1(CCObject* sender);
        void onBtn2(CCObject* sender);
};