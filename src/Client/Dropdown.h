#pragma once

#include "Client.h"

#include <Geode/Geode.hpp>
#include "CCContentSizeTo.h"

using namespace geode::prelude;

class Dropdown : public CCMenu {
    private:
        CCLabelBMFont* tex;
        CCSize size;
        CCScale9Sprite* bg;
        CCSprite* sprBtn;
        CCSprite* sprBtn2;
        CCMenuItemSpriteExtra* btn;

        int selected = 0;
        bool open;
        std::vector<std::string> strs;
        std::vector<CCMenuItemSprite*> btns;

        cocos2d::SEL_MenuHandler event;

    public:
        void onToggleVisible(CCObject*);

        void setVis(bool n);

        void onPress(CCObject* sender);

        bool init(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0);

        int getSelectedIndex() { return selected; }

        void setSelected(int sh)
        {
            selected = sh;

            auto ss = (strs[sh]);
            tex->setString(ss.c_str());
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
        }

        void visit();

        static Dropdown* create(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0);
};