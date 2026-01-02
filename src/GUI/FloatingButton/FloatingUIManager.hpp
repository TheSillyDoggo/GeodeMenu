#pragma once

#include <Geode/Geode.hpp>
#include "FloatingUIButton.hpp"

class FloatingUIManager
{
    protected:
        std::vector<FloatingUIButton*> buttons = {};
        FloatingUIButton* selected = nullptr;

        void sortButtons();

    public:
        static FloatingUIManager* get();

        void addButton(FloatingUIButton* btn);
        void removeButton(FloatingUIButton* btn);

        int getHighestButtonZ();

        void updateSprites();

        void visit();
        bool touches(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent, unsigned int uIndex);
};