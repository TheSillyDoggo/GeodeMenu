#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../../Client/Client.h"
#include "../SafeMode/SafeMode.hpp"

using namespace geode::prelude;

class $modify (NoclipPlayLayer, PlayLayer)
{
    struct Fields {
        GameObject* ac;
        GameObject* last;

        int t = 0;
        int d = 0;
        bool isDead = false;

        CCLayerColor* tint;

        Module* tintOnDeath;
        SliderModule* tintOpacity;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void destroyPlayer(PlayerObject* p0, GameObject* p1);
    void resetLevel();
};