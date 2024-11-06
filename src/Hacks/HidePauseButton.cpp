#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (UILayer)
{
    bool init(GJBaseGameLayer* p0)
    {
        if (!UILayer::init(p0))
            return false;

        if (auto menu = this->getChildByType<CCMenu>(0); auto btn = menu->getChildByType<CCMenuItemSpriteExtra>(0))
        {
            btn->getNormalImage()->setVisible(false);
        }

        return true;
    }

    QOLMOD_MOD_HOOK("hide-pause-button", "UILayer::init")
};