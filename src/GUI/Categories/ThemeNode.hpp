#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../AndroidUI.hpp"
#include "../Modules/BlurBackground.hpp"
#include "../Modules/UseColonThreeButton.hpp"
#include <BetterButtonSprite.hpp>

class ThemeNode : public CategoryNode
{
    protected:
        std::unordered_map<int, CCMenuItemSpriteExtra*> colourBtns;
        std::unordered_map<MenuAnimation, CCMenuItemSpriteExtra*> animBtns;
        CCScale9Sprite* colourBG = nullptr;
        ModuleNode* blurBG = nullptr;
        ModuleNode* colonThree = nullptr;
        BetterButtonSprite* btnSpr = nullptr;
        static inline ThemeNode* instance = nullptr;

        ~ThemeNode();

    public:
        CREATE_FUNC(ThemeNode)
        static ThemeNode* get();

        void onChangeAnim(CCObject* sender);
        void onChangeColour(CCObject* sender);
        void onPreviewAnim(CCObject* sender);
        void onChangeThemeOptions(CCObject* sender);

        CCMenuItemSpriteExtra* addColourBtn(int colour);
        void updateColourSprite();
        void updateCustomImageSprite();

        CCMenuItemSpriteExtra* addAnimBtn(MenuAnimation anim);
        CCNode* getAnimContainer(MenuAnimation anim);
        void updateAnimSprite();

        bool init();
};

SUBMIT_CATEGORY("Config/Theme", ThemeNode)