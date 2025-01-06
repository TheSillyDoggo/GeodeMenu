#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/MenuLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCNode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCScheduler.hpp>
#include <../../../build/bindings/bindings/Geode/modify/FMODLevelVisualizer.hpp>
#include "../../Client/Client.h"
#include "../../Utils/defines.hpp"

using namespace geode::prelude;

#ifdef QOLMOD_PULSINGMENU

Module* pulseAll = nullptr;

#ifdef GEODE_IS_WINDOWS
#define offset 0x210
#endif
#ifdef GEODE_IS_ANDROID32
#define offset 0x16c
#endif
#ifdef GEODE_IS_ANDROID64
#define offset 0x1f8
#endif
#ifdef GEODE_IS_MACOS
#define offset 0x1c8
#endif
#ifdef GEODE_IS_IOS
#define offset 0x1c8
#endif

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
            #ifdef GEODE_IS_WINDOWS
            MBO(bool, engine, 0x218) = true;
            #else
            engine->enableMetering();
            #endif

            engine->update(dt);
            
            float met = MBO(float, engine, offset);

            v = as<float>(std::lerp(as<float>(v), as<float>(met), dt * 6.9420f));
            if (node)
                node->setScale(mod->enabled ? (0.85f + clamp<float>(v * 0.25f, 0, 1)) : 1);
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
        mp->node = this->getChildByType<CCSprite>(0);

        this->addChild(mp);

        return true;
    }
};

#undef offset
#endif