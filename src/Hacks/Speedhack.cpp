#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

bool stringToFloat(const std::string& str, float& result) {
    std::istringstream iss(str);
    iss >> std::noskipws; // Disable skipping whitespace
    char c;
    if (!(iss >> result) || (iss >> c && !std::isspace(c))) {
        // Conversion failed or there are extra characters
        return false;
    }
    return true;
}

/*class $modify (CCScheduler)
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

                //if (!stringToFloat(SpeedhackTop::instance->text, v))
                    //v = 1.0f;

                try
                {
                    v = std::stof(SpeedhackTop::instance->text);
                }
                catch(const std::exception& e)
                {
                    //std::cerr << e.what() << '\n';
                    v = 1.0f;
                }
                    

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
        
        CCScheduler::update(dt * 2);
    }
}; // LINE 115 DOESN'T FUCKING EXIST???? HOW CRASH
*/

void myUpdate(CCScheduler* ins, float dt)
{
    if (!masterGroup)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);

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
            ins->update(dt * v);
            ColourUtility::update(dt * v);
            return;
        }
    }

    masterGroup->setPitch(1);
    ColourUtility::update(dt);
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