#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BetterSlider : public CCMenu
{
    protected:
        CCNode* target = nullptr;
        SEL_MenuHandler handler = nullptr;

        CCScale9Sprite* groove = nullptr;
        CCSprite* fill = nullptr;
        CCNode* thumbContainer = nullptr;
        CCSprite* thumbSpriteNormal = nullptr;
        CCSprite* thumbSpriteSelected = nullptr;
        CCNode* snapContainer = nullptr;
        std::vector<CCSprite*> snapSprites = {};

        bool selected = false;
        float selectOffsetX = 0;

        float snapRange = 4;
        std::vector<float> snapValues = {};
        bool useAltSnapSprite = false;

        float value = 0.5f;
        float rangeMin = 0;
        float rangeMax = 1;

        bool bypassAllowed = true;

        float value01ToXPos(float value, float inset = 5);
        void updateSnapSprites();
        void runCallback();

    public:
        CREATE_FUNC(BetterSlider)
        static BetterSlider* create(CCNode* target, SEL_MenuHandler handler);

        void setValue01(float value);
        float getValue01();

        void setRange(float min, float max);
        void setValueRanged(float value);
        float getValueRanged();

        void setSnapValues(std::vector<float> snapValues);
        void setSnapValuesRanged(std::vector<float> snapValues);
        void setUseAltSnapSprite(bool alt);

        void setBypassAllowed(bool bypass);

        CCNode* getThumb();

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

        virtual bool init();
        virtual void visit();
};