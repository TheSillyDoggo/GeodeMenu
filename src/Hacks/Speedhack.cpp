#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

bool stringToFloat(const std::string& str, float& result) {
    std::istringstream iss(str);
    if (!(iss >> result)) {
        // Conversion failed
        return false;
    }
    // Check if there are any trailing characters in the stream
    char remaining;
    if (iss >> remaining) {
        // Conversion succeeded, but there are extra characters in the string
        return false;
    }
    // Conversion succeeded
    return true;
}

class $modify (CCScheduler)
{
    void update(float dt)
    {
        if (!masterGroup)
        {
            FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
        }

        // icon effects value
        ColourUtility::update(dt);

        if (SpeedhackTop::instance)
        {
            if (SpeedhackEnabled::instance->enabled)
            {
                float v = 1.0f;

                if (!stringToFloat(SpeedhackTop::instance->text, v))
                    log::info("bruh: {}", SpeedhackTop::instance->text);

                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;

                bool m = SpeedhackMus::instance->enabled;

                if (SpeedhackGameplay::instance->enabled)
                    if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

                //FMOD::ChannelGroup* masterGroup;
                //0FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
                masterGroup->setPitch(m ? v : 1);
                
                CCScheduler::update(dt * v);
                return;
            }
        }

        masterGroup->setPitch(1);
        
        CCScheduler::update(dt);
    }
}; // LINE 115 DOESN'T FUCKING EXIST???? HOW CRASH