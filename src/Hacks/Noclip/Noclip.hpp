#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (NoclipPlayLayer, PlayLayer)
{
    struct Fields {
        GameObject* ac;
        GameObject* last;

        bool hasDied = false;
        float timeDead = 0;

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

    float getNoclipAccuracy();
};

class $modify (NoclipBaseGameLayer, GJBaseGameLayer)
{
    virtual void update(float dt);
};