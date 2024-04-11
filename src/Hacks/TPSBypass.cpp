#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

template <typename T>
inline std::vector<uint8_t> getBytes(T value) {
    //return std::vector<uint8_t>((uint8_t *) &value, (uint8_t *) &value + sizeof(T));
    std::vector<uint8_t> out(sizeof(float));
    float in = value;
    std::memcpy(out.data(), &in, out.size());

    return out;
}

std::vector<Patch*> patches = {};

void updateTPSPatches(bool tpsEnabled)
{    
    if (patches.size() > 0)
    {
        for (auto catgirl : patches)
        {
            Mod::get()->disownPatch(catgirl); // goodbye cutie you will be very missed :3c
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
        #ifdef GEODE_IS_WINDOWS
        patches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + 0x49D548), getBytes<float>(1.0f / tps)).unwrap());
        #endif

        #ifdef GEODE_IS_ANDROID32
        patches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0x467E6A - 0x10000)), getBytes<float>(1.0f / tps)).unwrap());
        #endif

        #ifdef GEODE_IS_ANDROID64
        patches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + 0x83355C), getBytes<float>(1.0f / tps)).unwrap());
        #endif
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