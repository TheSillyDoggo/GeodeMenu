#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"
#include "../Utils/OffsetManager.hpp"

using namespace geode::prelude;

#ifdef QOLMOD_TPS_BYPASS_HOOK

/*class $modify (GJBaseGameLayer)
{
    struct Fields
    {
        InputModule* input;
    };

    virtual bool init()
    {
        if (!GJBaseGameLayer::init())
            return false;

        m_fields->input = as<InputModule*>(Client::GetModule("tps-bypass")->options[0]);

        return true;
    }

    float getModifiedDelta(float dt)
    {
        auto m_resumeTimer = MBO(double, this, 0x329c);
        auto m_physDeltaBuffer = MBO(double, this, 0x3248);

        #define _resumeTimer = MBO(float, this, 0x329c)
        #define _physDeltaBuffer MBO(double, this, 0x3248)

        if (m_resumeTimer-- > 0)
        { 
            dt = 0.f;
        }

        float modifier = fminf(1.0, this->m_gameState.m_timeWarp) / 30.f;
        float total = dt + m_physDeltaBuffer;
        double result = (double)llroundf(total / modifier) * modifier;
        _physDeltaBuffer = total - result;

        log::info("m_resumeTimer: {}, m_physDeltaBuffer: {}, modifier: {}, result: {}", m_resumeTimer, m_physDeltaBuffer, modifier, result);

        return result;
    }

    QOLMOD_MOD_HOOK("tps-bypass", "GJBaseGameLayer::getModifiedDelta")
};*/

#else

template <typename T>
inline std::vector<uint8_t> getBytes(T value) {
    return std::vector<uint8_t>((uint8_t *) &value, (uint8_t *) &value + sizeof(T));
    //std::vector<uint8_t> out(sizeof(T));
    //T in = value;
    //std::memcpy(out.data(), &in, out.size());

    //return out;
}

std::vector<Patch*> patches = {};

void updateTPSPatches(bool tpsEnabled)
{    
    if (patches.size() > 0)
    {
        for (auto catgirl : patches)
        {
            if (catgirl)
                (void)Mod::get()->disownPatch(catgirl); // goodbye cutie you will be very missed :3c
        }

        patches.clear();
    }

    float tps = 240;

    auto x = numFromString<float>(as<InputModule*>(Client::GetModule("tps-bypass")->options[0])->text);

    if (x.isOk())
    {
        tps = x.value();
    }

    if (tpsEnabled)
    {
        if (auto offset = OffsetManager::get()->offsetForType(PatchType::PhysicsBypass); offset != 0x80085)
        {
            #ifdef GEODE_IS_WINDOWS
            auto array = geode::toBytes<float>(1.0f / tps);
            #else
            auto array = geode::toBytes<double>(1.0f / tps);
            #endif

            patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + offset), array));
        }
    }
}

class TPSChangedDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto del = new TPSChangedDelegate();

        Client::GetModule("tps-bypass")->delegate = del;
        Client::GetModule("tps-bypass")->options[0]->delegate = del;

        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    });
}

#endif