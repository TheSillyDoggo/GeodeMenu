#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Client.h"
#include "../Utils/CCBlurLayer.hpp"

using namespace geode::prelude;

class SillyBaseLayer : public geode::Popup<>
{
    public:
        CCMenu* baseLayer = nullptr;
        CCMenuItemSpriteExtra* ok = nullptr;
        CCPoint size = CCPointZero;
        CCBlurLayer* blur = nullptr;

        ~SillyBaseLayer();

        virtual void onClose(CCObject*)
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

        virtual bool setup();

        bool initWithSizeAndName(CCPoint size, std::string _title, bool createWithOK = true, bool animate = true, bool noBlur = false);
};
