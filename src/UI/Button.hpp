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
            std::function<void()> selectedCallback = nullptr;
            std::function<void()> unselectedCallback = nullptr;
            std::function<void()> activateCallback = nullptr;

            bool init(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);
            
            virtual void draw();
            virtual void update(float dt);

        public:
            float m_scaleMultiplier = 1.26f;

            static Button* create(CCNode* normalSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);
            static Button* create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);
            static Button* create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);

            void setRepeatEnabled(bool en);
            bool isRepeatEnabled();

            void setCallbacks(std::function<void()> selectedCallback, std::function<void()> unselectedCallback, std::function<void()> activateCallback);

            virtual void selected();
            virtual void unselected();
            virtual void activate();
    };
};