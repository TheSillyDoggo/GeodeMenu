#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"

using namespace geode::prelude;

class InputModule : public Module, public TextInputDelegate
{
    public:
        float speedhackV = 1.0f;
        static inline InputModule* selected = nullptr;

        std::string text = "";
        std::string format = "%";
        std::string allowedChars = "0987654321.";

        std::string lastCheckedText = "";
        float floatValue = 1;
        int intValue = 0;

        int maxSize = 5;

        TextInput* inputField = nullptr;

        InputModule();
        InputModule(std::string name, std::string id, std::string def);

        void updateValue();
        float getFloatValue();
        int getIntValue();

        void save();
        void load();

        void makeAndroid(CCNode* menu, CCPoint pos);

        virtual void drawImGui();

        virtual void textChanged(CCTextInputNode* input);
};