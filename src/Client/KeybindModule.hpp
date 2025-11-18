#pragma once

#include "Module.hpp"

class KeybindModule : public Module
{
    protected:
        int keyCode = -1;
        int defaultKeyCode = -1;

        virtual void save();
        virtual void load();

        void setDefaultKeyCode(int defaultCode);

    public:
        void setKeyCode(int code);
        int getKeyCode();

        int getDefaultKeyCode();

        virtual ModuleNode* getNode();
};