#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (NoclipPlayLayer, PlayLayer)
{
    struct Fields {
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

class $modify (NoclipBaseGameLayer, GJBaseGameLayer)
{
    struct Fields {
        GameObject* ac;
        
        float timeDead = 0;
        float timeInLevel = 0;
        bool hasDied = false;

        bool isTickUpdate = false;
    };

    void resetLevelVariables();

    void checkRepellPlayer();
    void updateCamera(float dt);

    float getNoclipAccuracy();

    bool shouldIncreaseTime();
};

class $modify (NoclipEditorLayer, LevelEditorLayer)
{
    virtual void playerTookDamage(PlayerObject* p0);
    virtual void postUpdate(float p0);
};