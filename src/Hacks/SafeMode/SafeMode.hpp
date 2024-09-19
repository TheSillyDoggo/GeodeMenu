#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../../Client/Client.h"
#include "../../Labels/Labels.h"

using namespace geode::prelude;

class HackModuleDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled);
};

class SpeedhackDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled);
};

class SILLY_DLL SafeMode
{
    private:
        bool hackedLevelLoad;
        bool hackedAttempt;
        bool hackedAttemptReal;

        static inline SafeMode* instance = nullptr;

    public:
        bool speedhackKick;
        bool safeEndScreen;

        std::vector<std::string> hacks = {
            "instant",
            "no-reverse",
            "no-static",
            "show-hitboxes",
            "coin-tracers",
            "show-trajectory",
            "rand-seed",
            "jump-hack",
            "tps-bypass",
            "auto-coins",
            "auto-clicker",
        };

        static SafeMode* get();

        void resetOnLevelLoad();
        void resetOnNewAttempt();

        void setHackedLoad();
        void setHackedAttempt();

        void updateSpeedhackShouldKick();

        void addDelegateToModules();
        ccColor3B colourForState();

        bool shouldKickFromLevel();
        void updateIndicator();
};

class $modify (SafePlayLayer, PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);

    void resetLevel();
    void levelComplete();
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5);
};

class $modify (SafeGJGameLevel, GJGameLevel)
{
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4);
};

class $modify (SafeEndLevelLayer, EndLevelLayer)
{
    virtual void customSetup();
};