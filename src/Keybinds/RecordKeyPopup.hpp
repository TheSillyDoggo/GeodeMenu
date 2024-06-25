#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RecordKeyPopup : public CCLayerColor
{
    public:
        SEL_MenuHandler handler;

        bool init(SEL_MenuHandler obj);
        virtual void keyDown(enumKeyCodes key);
        virtual void keyBackClicked();

        static RecordKeyPopup* create(SEL_MenuHandler obj);
};