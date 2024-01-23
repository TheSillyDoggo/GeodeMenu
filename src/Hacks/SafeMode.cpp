#include <Geode/Geode.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

class $modify (PlayLayer)
{
    TodoReturn levelComplete() {
        if (!Client::GetModuleEnabled("safe-mode"))
        {
            PlayLayer::levelComplete();
            return;
        }

        this->onQuit();
        FMODAudioEngine::sharedEngine()->playEffect("quitSound_01.ogg");
    }

    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        if (!Client::GetModuleEnabled("safe-mode"))
            return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }
};

class $modify(GJGameLevel) {
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        if (!Client::GetModuleEnabled("safe-mode"))
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
    }
};
