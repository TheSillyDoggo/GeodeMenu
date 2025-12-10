#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../AndroidUI.hpp"
#include "../Modules/BlurBackground.hpp"
#include "../Modules/UseColonThreeButton.hpp"

class ThemeNode : public CategoryNode
{
    protected:
        std::unordered_map<int, CCMenuItemSpriteExtra*> colourBtns;
        std::unordered_map<MenuAnimation, CCMenuItemSpriteExtra*> animBtns;
        CCScale9Sprite* colourBG = nullptr;
        ModuleNode* blurBG = nullptr;
        ModuleNode* colonThree = nullptr;

    public:
        CREATE_FUNC(ThemeNode)

        void onChangeAnim(CCObject* sender);
        void onChangeColour(CCObject* sender);
        void onPreviewAnim(CCObject* sender);
        void onChangeThemeOptions(CCObject* sender);

        CCMenuItemSpriteExtra* addColourBtn(int colour);
        void updateColourSprite();

        CCMenuItemSpriteExtra* addAnimBtn(MenuAnimation anim);
        CCNode* getAnimContainer(MenuAnimation anim);
        void updateAnimSprite();

        bool init();
};

SUBMIT_CATEGORY("Config/Theme", ThemeNode)