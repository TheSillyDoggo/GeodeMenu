#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include "../Utils/Utils.hpp"
#include <imgui-cocos.hpp>
#include "Module.h"

class Window : public UIComponent
{
    public:
        std::string name;
        std::string id;
        bool excludeAndroid = false;
        std::vector<Module*> modules;
        ImVec2 windowPos;
        Ref<ScrollLayer> scroll;

        bool dragging = false;
        CCPoint offset = CCPoint(0, 0);

        bool isClosed = false;
        float v = 1.0f;

        float quadraticEaseInOut(float t);

        virtual void setPosition(const CCPoint &position);
        virtual void drawImGui();

        //stolen from stack overflow
        int getIndex(std::vector<float> v, float K);
        float clampf(float v, float min, float max);

        bool a = false;
        int i = 0;

        CCPoint offsetForTime(float time);

        virtual void cocosCreate(CCMenu* menu);
};

#ifndef GEODE_IS_IOS

class $modify (WindowMouseDispatcher, CCMouseDispatcher)
{
    bool dispatchScrollMSG(float x, float y);
};

#endif