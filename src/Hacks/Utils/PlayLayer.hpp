#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify (PlayLayerUtils, PlayLayer)
{
    struct Fields
    {
        CCDrawNode* drawNode = nullptr;

        std::vector<EffectGameObject*> coins = {};
        std::vector<StartPosObject*> startPositions = {};
        std::vector<GameObject*> speedObjects = {};
        std::vector<GameObject*> mirrorObjects = {};
        std::vector<GameObject*> sizeObjects = {};
        std::vector<GameObject*> dualObjects = {};
        std::vector<GameObject*> gamemodeObjects = {};
        std::vector<GameObject*> gravityObjects = {};
    };

    static PlayLayerUtils* getUtils();

    void sortByXPos();
    float getCurrentPercentageAdv();
    int getDecimalsToShow();

    // hooks

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void addObject(GameObject* obj);
    void createObjectsFromSetupFinished();
    virtual void postUpdate(float p0);

    static void onModify(auto& self);
};