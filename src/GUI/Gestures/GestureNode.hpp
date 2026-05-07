#pragma once

#include <Geode/Geode.hpp>
#include <Touch.hpp>
#include <UserSprite.hpp>

namespace qolmod
{
    enum class GestureAnchor
    {
        Left,
        Top,
        Right,
        Bottom,
    };

    class GestureNode : public cocos2d::CCNodeRGBA
    {
        private:
            qolmod::GestureAnchor anchor = qolmod::GestureAnchor::Left;
            cocos2d::CCPoint displayedPosition = cocos2d::CCPointZero;
            cocos2d::CCPoint realPosition = cocos2d::CCPointZero;
            qolmod::UserSprite* cornerTop = nullptr;
            qolmod::UserSprite* cornerBottom = nullptr;
            cocos2d::CCLayerColor* sideFill = nullptr;
            cocos2d::CCLayerColor* gapFill = nullptr;

        public:
            void setAnchor(qolmod::GestureAnchor anchor);

            virtual bool touchBegan(qolmod::Touch* touch);
            virtual void touchMoved(qolmod::Touch* touch);
            virtual void touchEnded(qolmod::Touch* touch);
            virtual void touchCancelled(qolmod::Touch* touch);

            virtual bool init();
            virtual void update(float dt);
            virtual void setContentSize(const cocos2d::CCSize& contentSize);

            virtual void setOpacity(GLubyte opacity);
            virtual void setColor(const cocos2d::ccColor3B& color);
    };
};