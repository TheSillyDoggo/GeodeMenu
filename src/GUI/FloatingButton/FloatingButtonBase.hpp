#pragma once

#include <Geode/Geode.hpp>
#include <Touch.hpp>

class FloatingButtonBase : public cocos2d::CCNode
{
    public:
        enum class AnimationType : uint8_t
        {
            Shrink = 0,
            GD = 1,
        };
        enum class HitboxType : uint8_t
        {
            Circle = 0,
            Square = 1,
        };

    private:
        // used for internal button actions, do not use
        bool isMoving = false;
        float _opacity = 0.8f;
        cocos2d::CCPoint position = cocos2d::CCPointZero;
        bool isSelected = false;

    protected:
        // changes how the button behaves visually
        float scale = 0.8f;
        float opacity = 0.8f;
        bool isMovable = true;
        HitboxType hitboxType = HitboxType::Circle;
        AnimationType animation = AnimationType::Shrink;

        virtual void update(float dt);
        ~FloatingButtonBase();

        virtual bool doesTouchOverlap(cocos2d::CCPoint touchPoint);
        virtual void updateVisuals(float opacity);
        virtual void onClick();
        virtual void setupChildren();
        virtual void draw();

        void animate(bool release, bool clicked = false);

    public:
        static FloatingButtonBase* create();

        void setMovable(bool movable);
        void setBaseScale(float scale);
        void setBaseOpacity(float opacity);
        void setAnimation(AnimationType anim);

        virtual bool init();
        virtual bool ccTouchBegan(qolmod::Touch* touch);
        virtual void ccTouchMoved(qolmod::Touch* touch);
        virtual void ccTouchEnded(qolmod::Touch* touch);
        virtual void setPosition(cocos2d::CCPoint position, bool instant);
};