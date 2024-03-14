/*#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

using namespace geode::prelude;

class MenuPulse : public CCNode
{
    public:
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
            //auto v = MBO(float, FMODAudioEngine::sharedEngine(), 0x178);

            //log::info("volume: {}", v);
        }

        CREATE_FUNC(MenuPulse);
};

class $modify (MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        this->addChild(MenuPulse::create());

        return true;
    }
};*/