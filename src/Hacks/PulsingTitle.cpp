#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/FMODLevelVisualizer.hpp>
#include "../Client/Client.h"

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

using namespace geode::prelude;

class MenuPulse : public CCNode
{
    public:
        CCNode* node = nullptr;
        FMOD::Channel* channel = nullptr;

        bool init()
        {
            if (!CCNode::init())
                return false;

            channel = FMODAudioEngine::sharedEngine()->m_globalChannel;
            this->scheduleUpdate();

            return true;
        }

        void update(float dt)
        {
            #ifdef GEODE_IS_ANDROID
            node->setScale(FMODAudioEngine::sharedEngine()->getMeteringValue());
            #endif

            #ifdef GEODE_IS_WINDOWS
            //node->setScale(2);
            #endif
        }

        CREATE_FUNC(MenuPulse);
};

class $modify (FMODLevelVisualizer)
{
    TodoReturn updateVisualizer(float p0, float p1, float p2)
    {
        log::info("p0: {}, p1: {}, p2: {}", p0, p1, p2);

        FMODLevelVisualizer::updateVisualizer(p0, p1, p2);
    }
};

class $modify (MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        this->addChild(FMODLevelVisualizer::create());

        auto mp = MenuPulse::create();
        mp->node = getChildOfType<CCSprite>(this, 0);

        this->addChild(mp);

        return true;
    }
};