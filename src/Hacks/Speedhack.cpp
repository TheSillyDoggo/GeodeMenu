#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCScheduler)
{
    void update(float dt)
    {
        // icon effects value
        ColourUtility::update(dt);


        if (SpeedhackTop::instance)
        {
            if (Client::GetModuleEnabled("speedhack-enabled"))
            {
                float v = 1.0f;

                if (SpeedhackTop::instance->text.size() != 0 && !SpeedhackTop::instance->text.ends_with("."))
                {
                    v = std::stof(SpeedhackTop::instance->text);
                }

                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;

                if (Client::GetModuleEnabled("speedhack-gameplay"))
                    if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

                FMOD::ChannelGroup* masterGroup;
                FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
                masterGroup->setPitch(Client::GetModuleEnabled("speedhack-music") ? v : 1);
                
                CCScheduler::update(dt * v);
                return;
            }
        }

        FMOD::ChannelGroup* masterGroup;
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
        masterGroup->setPitch(1);
        
        CCScheduler::update(dt);
    }
};