#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.h"
#include "../Hacks/SafeMode/SafeMode.hpp"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class SwitchGamemodePopup : public SillyBaseLayer
{
    public:
        std::vector<CCMenuItemToggler*> speedBtns;
        std::vector<CCMenuItemToggler*> gamemodeBtns;
        CCMenuItemToggler* mirror;

        PlayerObject* getPlayer();
        int getGamemodeIndex(PlayerObject* player);

        void onSpeed(CCObject* sender);
        void onGamemode(CCObject* sender);
        void onMirror(CCObject* sender);

        void updateButtons();

        virtual void customSetup();

        static SwitchGamemodePopup* create();

        static SwitchGamemodePopup* addToScene();
};