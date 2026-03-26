#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class CCMenuExt : public cocos2d::CCMenu
    {
        protected:
            bool useArea = false;
            cocos2d::CCRect area = {0, 0, 0, 0};
            cocos2d::CCNode* linkedRect = nullptr;

        public:
            CREATE_FUNC(CCMenuExt)
            
            void setUseRect(bool use);
            void setRect(cocos2d::CCRect rect);
            void setLinkedRect(cocos2d::CCNode* node);

            virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
            virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
            virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event);
            virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    };
};