#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.hpp"

using namespace geode::prelude;

class SafeMode
{
    protected:
        bool isLevelLoadCheated = false;
        bool isAttemptCheated = false;
        std::vector<std::string> levelLoadReasons = {};
        std::vector<std::string> attemptReasons = {};

    public:
        static SafeMode* get();

        void onLevelLoad();
        void onLevelNewAttempt();

        void onModuleToggled(Module* mod);
        void onSpeedhackChanged();

        ccColor3B getIndicatorColour();

        bool shouldDisableLevelProgress();

        void addMessage(SafeModeTrigger level, std::string message);
};