#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"

using namespace geode::prelude;

class SmartStartposUIModule : public Module
{
    public:
        SmartStartposUIModule();

        void save();
        void load();

        virtual CCSize sizeForOptionsPage();

        void makeAndroid(CCNode* menu, CCPoint pos);
};