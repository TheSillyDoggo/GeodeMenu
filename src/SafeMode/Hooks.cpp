#include "Hooks.hpp"
#include "SafeMode.hpp"

bool SafePlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    SafeMode::get()->onLevelLoad();

    return true;
}

void SafePlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    SafeMode::get()->onLevelNewAttempt();
}

void SafePlayLayer::levelComplete()
{
    bool v = m_isTestMode;

    m_isTestMode = SafeMode::get()->shouldDisableLevelProgress();

    PlayLayer::levelComplete();

    m_isTestMode = v;
}

void SafePlayLayer::showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    SafePlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
}

void SafeGJGameLevel::savePercentage(int p0, bool p1, int p2, int p3, bool p4)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
}