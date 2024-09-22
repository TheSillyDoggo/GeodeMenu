#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"
#include "../Dropdown.h"

using namespace geode::prelude;

class FontModule : public Module
{
    private:
        std::string font;
    public:
        FontModule(std::string id);

        virtual void save();
        virtual void load();

        void makeAndroid(CCNode* menu, CCPoint pos);

        std::string getSelectedFont();

        void onChooseFont(CCObject* sender);
};