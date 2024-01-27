#include <Geode/Geode.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

void updateSafemode()
{
    if (!Client::GetModuleEnabled("auto-safe-mode"))
        return;

    std::vector<std::string> names = {
        "speedhack-enabled",
        "force-plat",
        "noclip",
        "instant"
    };

    bool a = false;

    for (size_t i = 0; i < names.size(); i++)
    {
        if (Client::GetModuleEnabled(names[i]))
        {
            a = true;

            break;
        }
    }
    
    Client::GetModule("safe-mode")->enabled = a;
}

class $modify (PlayLayer)
{
    TodoReturn levelComplete() {
        updateSafemode();

        if (!Client::GetModuleEnabled("safe-mode"))
        {
            PlayLayer::levelComplete();
            return;
        }

        this->onQuit();
        FMODAudioEngine::sharedEngine()->playEffect("quitSound_01.ogg");
    }

    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        updateSafemode();

        if (!Client::GetModuleEnabled("safe-mode"))
            return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }
};

class $modify(GJGameLevel) {
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        updateSafemode();

        if (!Client::GetModuleEnabled("safe-mode"))
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
    }
};
