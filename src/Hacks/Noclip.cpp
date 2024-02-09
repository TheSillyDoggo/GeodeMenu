#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    GameObject* ac;
    //int i = 0;
    //int d = 0;

    void destroyPlayer(PlayerObject * p0, GameObject * p1)
    {
        if (!m_fields->ac)
            m_fields->ac = p1;

        if (!Client::GetModuleEnabled("noclip") || (m_fields->ac == p1))
            PlayLayer::destroyPlayer(p0, p1);
    }
};