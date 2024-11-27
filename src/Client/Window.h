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
        Ref<ScrollLayer> scroll;

        bool dragging = false;
        CCPoint offset = CCPoint(0, 0);

        bool isClosed = false;
        float closedTimer = 1;
        ImVec2 windowPos;
        ImVec2 actualWindowPos;
        ImVec2 dragOffset;

        int priority = 0;
        int visualPriority = 0;

        std::function<void(CCMenu*, ScrollLayer*)> createPostHook;

        Window();

        float quadraticEaseInOut(float t);

        ImVec2 getDesiredWindowSize();

        virtual const CCPoint& getPosition();
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