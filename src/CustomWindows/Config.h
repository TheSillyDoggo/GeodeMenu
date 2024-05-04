#pragma once

#include "../Client/Client.h"
#include "../Utils/CCPastelLabelBMFont.hpp"

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

        static inline Ref<CCSprite> sprJoin = nullptr;
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

        CCMenuItemToggler* createTabButton(std::string name, int index);

        void createBtn(CCNode* menu, int i);
        
        void onDisableGP(CCObject* sender);

        void onSliderChanged(CCObject* sender);

        void onLink(CCObject* sender);

        void onDropdownChanged(CCObject*);

        void onChangeTab(CCObject*);

        void cocosCreate(CCMenu* menu);

        void onChangeFile(CCObject*);
};