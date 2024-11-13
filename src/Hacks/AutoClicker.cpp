#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

InputModule* delay;
InputModule* holdFor;
Module* player1;
Module* player2;

class $modify (AutoBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        int frames;
        int framesHeld;
        bool isHeld;
    };

    void checkRepellPlayer() // Runs every tick
    {
        GJBaseGameLayer::checkRepellPlayer();

        auto fcont = m_fields.self();

        fcont->frames++;

        if (fcont->frames >= delay->getIntValue())
        {
            fcont->frames = 0;
            fcont->framesHeld = 0;
        }

        if (fcont->frames == 0)
        {
            if (player1->enabled)
                this->handleButton(true, as<int>(PlayerButton::Jump), true);

            if (player2->enabled && m_player2 && m_gameState.m_isDualMode)
                this->GJBaseGameLayer::handleButton(true, as<int>(PlayerButton::Jump), false);

            fcont->isHeld = true;
        }

        if (fcont->framesHeld >= holdFor->getIntValue())
        {
            if (player1->enabled)
                this->GJBaseGameLayer::handleButton(false, as<int>(PlayerButton::Jump), true);

            if (player2->enabled && m_player2 && m_gameState.m_isDualMode)
                this->GJBaseGameLayer::handleButton(false, as<int>(PlayerButton::Jump), false);
            
            fcont->isHeld = false;
        }

        if (m_fields->isHeld)
            fcont->framesHeld++;
    }

    QOLMOD_MOD_ALL_HOOKS("auto-clicker")
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto clicker = Client::GetModule("auto-clicker");
        delay = as<InputModule*>(clicker->options[0]);
        holdFor = as<InputModule*>(clicker->options[1]);
        player1 = as<InputModule*>(clicker->options[2]);
        player2 = as<InputModule*>(clicker->options[3]);
    });

    log::info("c");
}