#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../BetterSlider.hpp"

using namespace geode::prelude;

class SpeedhackNode : public CategoryNode, public TextInputDelegate
{
    protected:
        CCMenuItemToggler* enabledBtn = nullptr;
        CCMenuItemToggler* musicBtn = nullptr;
        CCMenuItemToggler* gameplayBtn = nullptr;
        CCMenuItemSpriteExtra* trashBtn = nullptr;
        BetterSlider* slider = nullptr;
        TextInput* input = nullptr;

    public:
        CREATE_FUNC(SpeedhackNode)

        void onToggleEnabled(CCObject* sender);
        void onToggleMusic(CCObject* sender);
        void onToggleGameplay(CCObject* sender);
        void onTrash(CCObject* sender);
        void onSliderMoved(CCObject* sender);

        virtual void textChanged(CCTextInputNode* node);

        bool init();
};

SUBMIT_CATEGORY("Speedhack", SpeedhackNode)