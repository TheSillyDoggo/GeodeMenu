#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../../Layers/EditPositionLayer.hpp"
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (SteppedBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        std::vector<Ref<CheckpointObject>> states;
        bool paused = true;
        bool steppingUpdate = false;
    };

    bool getPausedUpdate();

    virtual void update(float dt);
    void checkRepellPlayer(); // Runs every tick

    void stepFrame();

    void storeState();
    void restoreState();

    QOLMOD_MOD_ALL_HOOKS("frame-stepper")
};

class $modify (SteppedUILayer, UILayer)
{
    struct Fields
    {
        CCMenuItemSpriteExtra* next;
        CCMenuItemSpriteExtra* prev;
        CCMenuItemSpriteExtra* pause;
        CCMenu* menu;
    };

    bool init(GJBaseGameLayer* bgl);

    void onTogglePaused(CCObject*);
    void onPreviousFrame(CCObject*);
    void onStepFrame(CCObject*);

    void updateUI();
};

class $modify (SteppedKeyboardDispatcher, CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat);

    QOLMOD_MOD_ALL_HOOKS("frame-stepper")
};