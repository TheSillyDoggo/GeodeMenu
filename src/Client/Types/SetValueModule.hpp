#pragma once

#include <Geode/Geode.hpp>
#include "../Client.h"

using namespace geode::prelude;

class SetValueModule : public Module
{
    public:
        TextInput* inp;
        TextInput* inpX;
        TextInput* inpY;
        std::string allowedChars = "1234567890-.";

        SetValueModule(std::string name, std::string id);

        void save();
        void load();

        void makeAndroid(CCNode* menu, CCPoint pos);
        void onSet(CCObject* sender);
        void onScaleToggle(CCObject* sender);
};