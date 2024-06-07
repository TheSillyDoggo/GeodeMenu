#ifdef QOLMOD_PULSINGMENU

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/FMODLevelVisualizer.hpp>
#include "../Client/Client.h"
#include "../Utils/defines.hpp"

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
            engine->enableMetering();
            engine->update(dt);
            
            #ifdef GEODE_IS_WINDOWS
            float met = engine->m_pulse3;
            #else
            float met = engine->getMeteringValue();
            #endif

            v = as<float>(std::lerp(as<float>(v), as<float>(met), dt * 6.9420f));
            if (node)
                node->setScale(mod->enabled ? (0.85f + clampf(v * 0.25f, 0, 1)) : 1);
        }

        CREATE_FUNC(MenuPulse);
};

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

#endif