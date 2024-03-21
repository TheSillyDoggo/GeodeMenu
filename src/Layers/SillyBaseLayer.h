#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Module.h"

using namespace geode::prelude;

//class Module;

class SillyBaseLayer : public CCLayerColor
{
    public:
        CCMenu* baseLayer = nullptr;
        CCMenuItemSpriteExtra* ok = nullptr;
        CCPoint size = CCPointZero;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        virtual void customSetup()
        {

        }

        bool initWithSizeAndName(CCPoint size, std::string _title, bool createWithOK = true, bool animate = true);
};
