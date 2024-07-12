#pragma once

#include <Geode/Geode.hpp>
#include "Module.h"
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "Replay.h"

#include "../Layers/SaveMacroPopup.h"
#include "../Layers/LoadMacroPopup.h"
#include "../Layers/EditStatusPositionLayer.h"
#include "../Layers/ReplayOptionsLayer.h"

#include "../Utils/Utils.hpp"

class Client;

class Window {
public:
    std::string name;
    std::string id;
    bool excludeAndroid = false;
    std::vector<Module*> modules;
    CCPoint windowPos = CCPoint(100, 100);

    bool dragging = false;
    CCPoint offset = CCPoint(0, 0);

    bool isClosed = false;
    float v = 1.0f;

    bool touchBegan(CCPoint point, CCTouch* touch);
    bool touchMoved(CCPoint point, CCTouch* touch);
    bool touchEndedOrCancelled(CCPoint point, CCTouch* touch, bool cancelled);

    float quadraticEaseInOut(float t);

    void preDraw();
    void drawWindow();
    void postDraw();

    //stolen from stack overflow
    int getIndex(std::vector<float> v, float K);
    float clampf(float v, float min, float max);

    bool a = false;
    int i = 0;

    CCPoint offsetForTime(float time);

    virtual void cocosCreate(CCMenu* menu);
};