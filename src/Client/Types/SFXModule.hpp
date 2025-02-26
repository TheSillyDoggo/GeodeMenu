#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"
#include "../Dropdown.h"

using namespace geode::prelude;

class SFXModule : public Module
{
    public:
        std::vector<std::string> sfxs;
    
    public:
        SFXModule(std::string name, std::string id);

        virtual void save();
        virtual void load();

        void makeAndroid(CCNode* menu, CCPoint pos);

        void onPickSFX(CCObject* sender);

        void playRandomSFX();

        int getSFXCount();
        std::string getRandomSFX();
};