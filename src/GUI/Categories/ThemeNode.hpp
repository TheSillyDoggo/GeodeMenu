#pragma once

#include "../../Client/CategoryNode.hpp"

class ThemeNode : public CategoryNode
{
    protected:
        std::unordered_map<int, CCMenuItemSpriteExtra*> colourBtns;
        CCScale9Sprite* colourBG = nullptr;

    public:
        CREATE_FUNC(ThemeNode)

        CCMenuItemSpriteExtra* addColourBtn(int colour);
        void onChangeColour(CCObject* sender);
        void updateColourSprite();

        bool init();
};

SUBMIT_CATEGORY("Config/Theme", ThemeNode)