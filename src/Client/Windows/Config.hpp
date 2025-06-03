#pragma once

#include "../Client.h"
#include "../Dropdown.h"
#include "../../Utils/CCPastelLabelBMFont.hpp"
#include "../../UI/PlayerDeathAnimation.hpp"

class Config : public Window
{
    public:
        Config()
        {
            name = "Config";
            id = "config-window";
        }

        static inline std::vector<CCNode*> tabs = {};
        static inline int selectedTab = 1;

        static inline CCSprite* sprJoin = nullptr;
        CCMenu* menu = nullptr;

        static inline std::vector<CCScale9Sprite*> btns = {};
        static inline std::vector<CCMenuItemSpriteExtra*> btnsS = {};
        static inline Slider* normal = nullptr;
        static inline Slider* GP = nullptr;
        static inline Slider* ED = nullptr;
        static inline Slider* scale = nullptr;
        static inline Dropdown* animDropdown = nullptr;

        static inline CCMenu* btnMenu = nullptr;
        static inline CCSprite* btn = nullptr;
        static inline CCLabelBMFont* btnL = nullptr;

        void changeTheme(CCObject* sender);

        virtual void drawImGui();

        CCMenuItemToggler* createTabButton(std::string name, int index);

        void createBtn(CCNode* menu, int i);
        
        void onDisableEditor(CCObject* sender);
        void onDisableGP(CCObject* sender);
        void onSliderChanged(CCObject* sender);
        void onLink(CCObject* sender);
        void onDropdownChanged(CCObject*);
        void onChangeFile(CCObject*);
        void onChangeTab(CCObject*);
        void onManageKeybinds(CCObject*);
        void onChangeLanguage(CCObject*);
        void onRoxi(CCObject* sender);

        void cocosCreate(CCMenu* menu);
};