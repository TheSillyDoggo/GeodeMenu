#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (Client::GetModuleEnabled("force-plat"))
        {
            m_player1->togglePlatformerMode(true);

            if (m_player2)
                m_player2->togglePlatformerMode(true);
        }

        return true;
    }
};