#pragma once

#include "FloatingButtonBase.hpp"

class Module;
namespace qolmod
{
    class ShortcutSprite;
};
class ShortcutVisualConfig;

class ModuleShortcutButton : public FloatingButtonBase
{
    protected:
        ShortcutVisualConfig* conf = nullptr;
        Module* mod = nullptr;
        bool lastUpdated = false;
        qolmod::ShortcutSprite* offSprite = nullptr;
        qolmod::ShortcutSprite* onSprite = nullptr;
        cocos2d::CCLabelBMFont* overlayLabel = nullptr;
        cocos2d::CCSprite* overlayBuiltIn = nullptr;
        cocos2d::CCSprite* overlayCustom = nullptr;
        cocos2d::ccColor3B lastColour = cocos2d::ccWHITE;

        virtual void updateVisuals(float opacity);
        virtual void onClick();

    public:
        static ModuleShortcutButton* create(Module* module);

        void updateSettings();

        void setup();
};