#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        m_player1->toggleVisibility(false);
        
        if (m_player2)
            m_player2->toggleVisibility(false);
    }

    QOLMOD_MOD_ALL_HOOKS("hide-player")
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        m_player1->toggleVisibility(true);
        
        if (m_player2)
            m_player2->toggleVisibility(m_gameState.m_isDualMode);
    }

    QOLMOD_MOD_ALL_HOOKS("show-player")
};