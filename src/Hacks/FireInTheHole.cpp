/*#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (FMODAudioEngine)
{
    static void onModify(auto& self) {
        auto res = self.getHook("FMODAudioEngine::playEffect");
        if (!res) {
            log::error("Something went horribly wrong");
            return;
        }
        log::info("I hooked {}!", res.unwrap()->getDisplayName());
    }

    void playEffect(gd::string p0, float p1, float p2, float p3)
    {
        log::info(p0);

        FMODAudioEngine::playEffect(p0, p1, p2, p3);
    }
};*/