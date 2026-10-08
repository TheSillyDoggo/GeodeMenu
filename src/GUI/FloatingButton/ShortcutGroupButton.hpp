#pragma once

#include "FloatingButtonBase.hpp"

class Module;
class ModuleShortcutButton;

class ShortcutGroupButton : public FloatingButtonBase
{
    protected:
        uint8_t groupID = 0;
        bool open = false;

        virtual void onClick();

    public:
        static ShortcutGroupButton* create(uint8_t group);

        bool isOpen();
};