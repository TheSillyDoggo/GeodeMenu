#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        // Toggle platformer mode for all relevant players and layers
        if (m_player1) m_player1->togglePlatformerMode(true);
        if (m_player2) m_player2->togglePlatformerMode(true);

        #ifdef GEODE_IS_MOBILE
        if (m_uiLayer) m_uiLayer->togglePlatformerMode(true);
        #endif

        return true;
    }

    QOLMOD_MOD_HOOK("force-plat", "PlayLayer::init")
};
