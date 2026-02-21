#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../IconicPlayerHook.hpp"

using namespace geode::prelude;

class $modify (IconicBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        IconicPlayerHook* hookP1 = nullptr;
        IconicPlayerHook* hookP2 = nullptr;
    };

    void createPlayer()
    {
        GJBaseGameLayer::createPlayer();

        if (m_player1)
        {
            m_fields->hookP1 = IconicPlayerHook::create(m_player1);
            m_fields->hookP1->setGamemode(IconicGamemodeType::Cube, false);
            m_player1->addChild(m_fields->hookP1);
        }

        if (m_player2)
        {
            m_fields->hookP2 = IconicPlayerHook::create(m_player2);
            m_fields->hookP2->setGamemode(IconicGamemodeType::Cube, true);
            m_player2->addChild(m_fields->hookP2);
        }
    }
};