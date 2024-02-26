#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

template <class Num>
Result<Num> anumFromString(std::string_view const str, int base = 10) {
    if constexpr (std::is_floating_point_v<Num> 
        #if defined(__cpp_lib_to_chars)
            && false
        #endif
    ) {
        Num val;
        char* strEnd;
        errno = 0;
        if (std::setlocale(LC_NUMERIC, "C")) {
            if constexpr (std::is_same_v<Num, float>) val = std::strtof(str.data(), &strEnd);
            else if constexpr (std::is_same_v<Num, double>) val = std::strtod(str.data(), &strEnd);
            else if constexpr (std::is_same_v<Num, long double>) val = std::strtold(str.data(), &strEnd);
            if (errno == ERANGE) return Err("Number is too large to fit");
            else if (strEnd == str.data()) return Err("String is not a number");
            else return Ok(val);
        }
        else return Err("Failed to set locale");
    }
    else {
        Num result;
        std::from_chars_result res;
        if constexpr (std::is_floating_point_v<Num>) res = std::from_chars(str.data(), str.data() + str.size(), result);
        else res = std::from_chars(str.data(), str.data() + str.size(), result, base);

        auto [_, ec] = res;
        if (ec == std::errc()) return Ok(result);
        else if (ec == std::errc::invalid_argument) return Err("String is not a number");
        else if (ec == std::errc::result_out_of_range) return Err("Number is too large to fit");
        else return Err("Unknown error");
    }
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

                auto x = anumFromString<float>(SpeedhackTop::instance->text);

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
};