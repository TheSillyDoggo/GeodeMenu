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
    };

    static PlayLayerUtils* getUtils();

    void addObject(GameObject* obj);
    virtual void postUpdate(float p0);

    static void onModify(auto& self);

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
};