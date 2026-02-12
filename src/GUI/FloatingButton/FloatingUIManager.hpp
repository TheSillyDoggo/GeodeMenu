#pragma once

#include <Geode/Geode.hpp>
#include "FloatingUIButton.hpp"

class FloatingUIManager : public cocos2d::CCNode
{
    friend struct FloatingMenuLayer;

    protected:
        std::vector<FloatingUIButton*> buttons = {};
        std::unordered_map<cocos2d::CCTouch*, FloatingUIButton*> trackingTouches = {};

        void sortButtons();

    public:
        static FloatingUIManager* get();

        void addButton(FloatingUIButton* btn);
        void removeButton(FloatingUIButton* btn);

        int getHighestButtonZ();

        void updateSprites();

        virtual void visit();
        bool touches(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent, unsigned int uIndex);
};