#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID

class $modify (GJBaseGameLayer)
{
    float getModifiedDelta(float dt)
    {
        auto v = GJBaseGameLayer::getModifiedDelta(dt);

        CCScene::get()->addChild(TextAlertPopup::create(fmt::format("dt: {} | unmod: {}", v, v * (1 / 240)).c_str(), 0.5f, 0.6f, 150, ""), 9999999);

        return v;
    }
};

#endif