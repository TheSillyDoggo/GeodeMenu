#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/FMODLevelVisualizer.hpp>
#include "../Client/Client.h"

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

using namespace geode::prelude;

Module* pulseAll = nullptr;

class MenuPulse : public CCNode
{
    public:
        Module* mod = nullptr;

        CCNode* node = nullptr;
        float v = 1.0f;

        FMODAudioEngine* engine = nullptr;

        bool init()
        {
            if (!CCNode::init())
                return false;

            mod = Client::GetModule("menu-pulse");

            engine = FMODAudioEngine::sharedEngine();

            this->scheduleUpdate();

            return true;
        }

        void update(float dt)
        {
            engine->updateMetering();

            v = as<float>(std::lerp(as<float>(v), as<float>(engine->getMeteringValue()), dt * 6.9420f));
            if (node)
                node->setScale(mod->enabled ? (0.85f + clampf(v * 0.25f, 0, 1)) : 1);
        }

        CREATE_FUNC(MenuPulse);
};

#ifndef GEODE_IS_ANDROID

float vPulse = 0;
Module* pul = nullptr;

class $modify (PulsingScheduler, CCScheduler)
{
    void update(float dt)
    {
        CCScheduler::update(dt);

        if (auto scene = CCScene::get())
        {
            if (!pul)
                pul = Client::GetModule("all-pulse");

            FMODAudioEngine::sharedEngine()->enableMetering();
        
            vPulse = as<float>(std::lerp(as<float>(vPulse), as<float>(FMODAudioEngine::sharedEngine()->getMeteringValue()), 0.1f));

            scene->setScale((pul->enabled && !PlayLayer::get()) ? (0.85f + clampf(vPulse * 0.25f, 0, 1)) : 1);
        }
    }
};

#endif

class $modify (MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        FMODAudioEngine::sharedEngine()->enableMetering();

        auto mp = MenuPulse::create();
        mp->node = getChildOfType<CCSprite>(this, 0);

        this->addChild(mp);

        return true;
    }
};