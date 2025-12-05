#pragma once

#include "Module.hpp"

class KeybindModule : public Module
{
    protected:
        int keyCode = -1;
        int defaultKeyCode = -1;
        bool mustHaveBind = false;

        virtual void save();
        virtual void load();

        void setDefaultKeyCode(int defaultCode);

    public:
        void setKeyCode(int code);
        int getKeyCode();

        int getDefaultKeyCode();
        
        bool isBindRequired();
        void setBindRequired(bool req);

        virtual ModuleNode* getNode();
};