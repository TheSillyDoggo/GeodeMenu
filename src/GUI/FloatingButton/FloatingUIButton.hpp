#pragma once

#include <Geode/Geode.hpp>

struct FloatingUIButtonVisibility
{
    bool showInMenu = true;
    bool showInGame = true;
    bool showInPauseMenu = true;
    bool showInEditor = true;
    bool showInEditorPauseMenu = true;

    bool shouldShow();
};

class FloatingUIButton : public cocos2d::CCNode
{
    protected:
        float scale = 0.8f;
        float opacity = 0.8f;
        bool movable = true;
        FloatingUIButtonVisibility visibilityConf = {};
        bool backgroundUsesSpriteSheet = false;
        bool overlayUsesSpriteSheet = false;

        std::function<void()> onClick = nullptr;
        std::string background = "";
        std::string overlay = "";
        bool isMoving = false;
        float _opacity = 0.8f;
        cocos2d::CCPoint position = cocos2d::CCPointZero;
        bool isSelected = false;

        virtual void update(float dt);
        ~FloatingUIButton();

    public:
        static FloatingUIButton* create(std::function<void()> onClick);

        bool init();

        void setOnClick(std::function<void()> onClick);
        void setButtonVisibilityConfig(FloatingUIButtonVisibility conf);
        void setMovable(bool movable);
        void setBaseScale(float scale);
        void setBaseOpacity(float opacity);

        void updateSprites(std::string background, std::string overlay, bool backgroundSpriteSheet, bool overlaySpriteSheet);
        void updateSprites();

        virtual void updatePosition(cocos2d::CCPoint point);

        bool ccTouchBegan(cocos2d::CCTouch* touch);
        void ccTouchMoved(cocos2d::CCTouch* touch);
        void ccTouchEnded(cocos2d::CCTouch* touch);
};