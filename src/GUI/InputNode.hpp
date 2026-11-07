#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "InputNode.hpp"
#include "EasyBG.hpp"

using namespace geode::prelude;

namespace qolmod
{
    class InputNode : public geode::TextInput
    {
        protected:
            bool initWithWidth(float width, std::string placeholder, ZStringView font);
            bool initWithSize(cocos2d::CCSize const& size, std::string placeholder, ZStringView font, bool multiline);

        public:
            static InputNode* create(float width, std::string placeholder, ZStringView font = "bigFont.fnt");
            static InputNode* create(cocos2d::CCSize const& size, std::string placeholder, ZStringView font = "bigFont.fnt");
            static InputNode* createMultiline(cocos2d::CCSize const& size, std::string placeholder, ZStringView font = "bigFont.fnt");

            void setLanguageFonts();
    };
};