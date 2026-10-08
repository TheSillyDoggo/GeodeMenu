#pragma once

#include "FloatingButtonBase.hpp"

class Module;
namespace qolmod
{
    class ShortcutSprite;
};

class ModuleShortcutButton : public FloatingButtonBase
{
    protected:
        Module* mod = nullptr;
        bool lastUpdated = false;
        qolmod::ShortcutSprite* offSprite = nullptr;
        qolmod::ShortcutSprite* onSprite = nullptr;

        virtual void updateVisuals(float opacity);
        virtual void onClick();

    public:
        static ModuleShortcutButton* create(Module* module);

        void updateSettings();

        void setup();
        virtual void update(float dt);
};