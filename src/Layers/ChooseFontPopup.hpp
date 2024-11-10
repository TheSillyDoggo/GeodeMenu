#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.h"
#include "../Hacks/SafeMode/SafeMode.hpp"
#include "../UI/ThreadedLabelBMFont.hpp"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class ChooseFontPopup : public SillyBaseLayer
{
    public:
        std::function<void(std::string)> callback; // callback is run whenever the value is changed by the user
        int selectedIndex = 0;
        std::vector<CCMenuItemToggler*> toggles;

        void setSelected(int id);
        void setSelected(std::string font); // selects the node based of a font name, if the font does not exist just sets it to bigFont

        void onSelect(CCObject* sender);

        virtual void customSetup();

        static ChooseFontPopup* create(std::function<void(std::string)> callback);
        static ChooseFontPopup* addToScene(std::function<void(std::string)> callback);
};