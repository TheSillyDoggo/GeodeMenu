#pragma once

#include <Geode/Geode.hpp>
#include "FloatingButtonBase.hpp"
#include <Touch.hpp>

class FloatingUIManager : public cocos2d::CCNode
{
    friend struct FloatingMenuLayer;

    protected:
        std::vector<FloatingButtonBase*> buttons = {};
        std::unordered_map<int, FloatingButtonBase*> trackingTouches = {};

        void sortButtons();

    public:
        static FloatingUIManager* get();

        void addButton(FloatingButtonBase* btn);
        void removeButton(FloatingButtonBase* btn);

        int getHighestButtonZ();

        virtual void visit();

        bool touchBegan(qolmod::Touch* touch);
        bool touchMoved(qolmod::Touch* touch);
        bool touchEnded(qolmod::Touch* touch);
        bool touchCancelled(qolmod::Touch* touch);
};