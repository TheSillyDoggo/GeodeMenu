#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class Button : public cocos2d::CCMenuItemSprite
    {
        protected:
            bool repeatEnabled = false;
            float t = 0;
            float v = 0;
            float d = 0;

            bool init(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);

        public:
            float m_scaleMultiplier = 1.26f;

            static Button* create(CCNode* normalSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);
            static Button* create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);
            static Button* create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);

            void setRepeatEnabled(bool en);
            bool isRepeatEnabled();

            virtual void selected();
            virtual void unselected();
            virtual void activate();
            virtual void update(float dt);
    };
};