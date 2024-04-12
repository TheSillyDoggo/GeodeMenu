#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

float speedhackLogic(float dt)
{
    if (!masterGroup)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);

    ColourUtility::totalSessionTime += dt;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            float v = 1.0f;

            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
            }

            if (v < 0.01f)
                v = 0.01f;

            if (v > 99999)
                v = 99999;

            bool m = SpeedhackMus::instance->enabled;

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            masterGroup->setPitch(SpeedhackMus::instance->enabled ? v : 1);
            ColourUtility::update(dt * v);
            return dt * v;
        }
    }

    masterGroup->setPitch(1);
    ColourUtility::update(dt);
    return dt;
}

void myUpdate(CCScheduler* ins, float dt)
{
    dt = speedhackLogic(dt);
    
    ins->update(dt);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCScheduler::update)
        ),
        &myUpdate,
        "cocos2d::CCScheduler::update",
        tulip::hook::TulipConvention::Thiscall
    );
}