#pragma once

#include "../Client/Client.h"

class Config : public Window
{
    public:
        Config()
        {
            name = "Config";
            id = "config-window";
        }

        static inline std::vector<CCScale9Sprite*> btns = {};
        static inline std::vector<CCMenuItemSprite*> btnsS = {};
        static inline Slider* normal = nullptr;
        static inline Slider* GP = nullptr;
        static inline Slider* ED = nullptr;
        static inline Slider* scale = nullptr;
        static inline Dropdown* dd = nullptr;

        void changeTheme(CCObject* sender);

        void createBtn(CCNode* menu, int i);

        void onSliderChanged(CCObject* sender);

        void onLink(CCObject* sender);

        void onDropdownChanged(CCObject*);

        void cocosCreate(CCMenu* menu);
};