#pragma once

#include "Module.hpp"

using namespace geode::prelude;

class ModBoolModule : public Module
{
    protected:
        Mod* mod = nullptr;
        std::string setting = "";
        bool invert = false;

        void setOptionPointer(std::string mod, std::string setting);
        void setInvert(bool invert);
        
    public:
        virtual void setUserEnabled(bool enabled);
        virtual bool getUserEnabled();

        virtual void save();
        virtual void load();
};