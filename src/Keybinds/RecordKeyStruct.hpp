#pragma once

#include <Geode/Geode.hpp>
#include "KeyStruct.hpp"

using namespace geode::prelude;

class RecordKeyStruct : public CCLayerColor
{
    public:
        std::function<void(KeyStruct)> callback;
        CCLabelBMFont* shift;
        CCLabelBMFont* ctrl;
        CCLabelBMFont* alt;
        CCLabelBMFont* cmd;

        bool init(std::function<void(KeyStruct)> obj);

        virtual void keyDown(enumKeyCodes key);
        virtual void keyBackClicked();
        virtual void update(float dt);

        static RecordKeyStruct* create(std::function<void(KeyStruct)> obj);
};