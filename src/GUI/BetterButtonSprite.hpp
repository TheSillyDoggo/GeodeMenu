#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class BetterButtonSprite : public CCNode
{
    protected:
        CCScale9Sprite* background = nullptr;
        AdvLabelBMFont* label = nullptr;

    public:
        CREATE_FUNC(BetterButtonSprite);
        static BetterButtonSprite* create(CCSize size, std::string str, std::string fnt, std::string bg);

        void setBGSprite(std::string file);
        void setFont(std::string font);
        void setString(std::string str);

        virtual bool init();
        virtual void visit();
};