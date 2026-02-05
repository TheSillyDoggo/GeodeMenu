#include "Hooks.hpp"
#include "Nodes/LabelContainerLayer.hpp"

bool LabelPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_uiLayer->addChild(LabelContainerLayer::create());

    return true;
}

void LabelPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    if (LabelContainerLayer::get())
        LabelContainerLayer::get()->onNewAttempt();
}

void LabelBaseGameLayer::handleButton(bool down, int button, bool isPlayer1, double timestamp)
{
    GJBaseGameLayer::handleButton(down, button, isPlayer1, timestamp);

    if (LabelContainerLayer::get() && down && button == 1)
        LabelContainerLayer::get()->onPlayerClicked(isPlayer1 ? NoclipPlayerSelector::Player1 : NoclipPlayerSelector::Player2);

    if (auto ll = LabelContainerLayer::get())
    {
        ll->onEventTriggered(down ? LabelEventType::ClickStarted : LabelEventType::ClickEnded);

        if (isPlayer1)
            ll->onEventTriggered(down ? LabelEventType::P1ClickStarted : LabelEventType::P1ClickEnded);
        else
            ll->onEventTriggered(down ? LabelEventType::P2ClickStarted : LabelEventType::P2ClickEnded);
    }
}