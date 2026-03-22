#pragma once

#include <Geode/Geode.hpp>

enum class MenuAnimation
{
    None,
    FromTop,
    FromBottom,
    FromLeft,
    FromRight,
    Scale,
    FadeIn
};

class PopupBase : public cocos2d::CCLayerColor, public cocos2d::CCActionTweenDelegate
{
    protected:
        cocos2d::CCSize m_size;
        bool m_noElasticity = false;
        cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
        cocos2d::CCMenu* m_buttonMenu = nullptr;
        cocos2d::CCLayerColor* blurLayer = nullptr;
        float drawOpacity = 1;
        geode::Ref<cocos2d::CCRenderTexture> rt = nullptr;
        int baseOpacity = 150;

        // pre 2.2081 compatibility
        virtual void keyDown(cocos2d::enumKeyCodes code);
        virtual void keyDown(cocos2d::enumKeyCodes code, double timestamp);
        virtual void keyBackClicked();
        virtual void registerWithTouchDispatcher(void);
        PopupBase();
        ~PopupBase();

        void onBtn1(CCObject* sender);
        void onBtn2(CCObject* sender);

        virtual void updateTweenAction(float value, const char* key);

        float calcPopupScale();
        void updateUIScale();
        void updateBlur();

    public:
        bool addToScene = true;
        bool shouldVisit = true;
        cocos2d::CCLayer* m_mainLayer = nullptr;

        static void updateAllUIScale();
        static void updateAllBlur();

        virtual bool init(float width, float height);
        virtual bool initAnchored(float width, float height);
        virtual bool setup();
        virtual void show();
        virtual void visit();

        void playAnimation(MenuAnimation anim);
        void onClose(CCObject* sender);
};