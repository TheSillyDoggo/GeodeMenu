#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/ModuleShortcutConfig.hpp"
#include "BetterInputNode.hpp"
#include "PopupBase.hpp"
#include "../FloatingShortcuts/ShortcutManager.hpp"

using namespace geode::prelude;

class EditShortcutConfigUI : public PopupBase
{
    protected:
        qolmod::BackgroundSprite* bg;
        Module* mod = nullptr;
        ShortcutVisualConfig* config = nullptr;

        CCMenu* createSectionedMenu(std::string title);

    public:
        static EditShortcutConfigUI* create(Module* mod);

        void onClose(CCObject* sender);

        virtual bool setup();
        virtual void update(float dt);
};