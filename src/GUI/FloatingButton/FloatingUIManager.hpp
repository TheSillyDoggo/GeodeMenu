#pragma once

#include <Geode/Geode.hpp>
#include "FloatingUIButton.hpp"
#include <Touch.hpp>

class FloatingUIManager : public cocos2d::CCNode
{
    friend struct FloatingMenuLayer;

    protected:
        std::vector<FloatingUIButton*> buttons = {};
        std::unordered_map<int, FloatingUIButton*> trackingTouches = {};

        void sortButtons();

    public:
        static FloatingUIManager* get();

        void addButton(FloatingUIButton* btn);
        void removeButton(FloatingUIButton* btn);

        int getHighestButtonZ();

        void updateSprites();

        virtual void visit();

        bool touchBegan(qolmod::Touch* touch);
        bool touchMoved(qolmod::Touch* touch);
        bool touchEnded(qolmod::Touch* touch);
        bool touchCancelled(qolmod::Touch* touch);
};