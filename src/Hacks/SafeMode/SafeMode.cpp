#include "SafeMode.hpp"

SafeMode* SafeMode::get()
{
    if (!instance)
        instance = new SafeMode();

    return instance;
}

void SafeMode::resetOnLevelLoad()
{
    hackedLevelLoad = false;
    hackedAttempt = false;
    hackedAttemptReal = false;

    updateIndicator();
}

void SafeMode::resetOnNewAttempt()
{
    hackedAttempt = Client::GetModuleEnabled("auto-safe");
    hackedAttemptReal = Client::GetModuleEnabled("auto-safe");

    bool autosafe = Client::GetModuleEnabled("auto-safe-mode");

    for (auto hack : hacks)
    {
        if (Client::GetModuleEnabled(hack))
        {
            if (autosafe)
                hackedAttempt = true;

            hackedAttemptReal = true;
        }
    }

    updateIndicator();
}

void SafeMode::setHackedLoad()
{
    hackedLevelLoad = true;

    updateIndicator();
}

void SafeMode::setHackedAttempt()
{
    if (Client::GetModuleEnabled("auto-safe-mode"))
        hackedAttempt = true;
    
    hackedAttemptReal = true;
    updateIndicator();
}

void SafeMode::addDelegateToModules()
{
    for (auto hack : hacks)
    {
        Client::GetModule(hack)->delegate = new HackModuleDelegate();
    }

    SpeedhackEnabled::instance->delegate = new SpeedhackDelegate();
    SpeedhackTop::instance->delegate = new SpeedhackDelegate();

    updateSpeedhackShouldKick();
}

ccColor3B SafeMode::colourForState()
{
    if (Client::GetModuleEnabled("safe-mode"))
        return ccc3(255, 255, 0);

    if (hackedAttemptReal || hackedLevelLoad || speedhackKick)
        return ccc3(255, 0, 0);

    return ccc3(0, 255, 0);
}

bool SafeMode::shouldKickFromLevel()
{
    return hackedAttempt || hackedLevelLoad || speedhackKick || Client::GetModuleEnabled("safe-mode");
}

void SafeMode::updateIndicator()
{
    if (!PlayLayer::get())
        return;

    #ifdef STATUS_TEXTS

    if (auto a = StatusNode::get())
    {
        if (auto l = a->sLabels[0])
            l->setColor(colourForState());

        a->update(1);
        a->reorderSides();
        a->reorderPosition();
    }

    #endif
}

void SafeMode::updateSpeedhackShouldKick()
{
    speedhackKick = Client::GetModuleEnabled("auto-safe-mode") ? (SpeedhackEnabled::instance->enabled ? (SpeedhackTop::instance->getFloatValue() < 1) : false) : false;
}

void HackModuleDelegate::onModuleChanged(bool enabled)
{
    SafeMode::get()->setHackedAttempt();
    SafeMode::get()->updateIndicator();
}

void SpeedhackDelegate::onModuleChanged(bool enabled)
{
    SafeMode::get()->updateSpeedhackShouldKick();

    if (SafeMode::get()->speedhackKick)
        SafeMode::get()->setHackedAttempt();
    
    SafeMode::get()->updateIndicator();
}

// hooks :3

bool SafePlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    SafeMode::get()->resetOnLevelLoad();
    SafeMode::get()->resetOnNewAttempt();

    if (Client::GetModuleEnabled("force-plat"))
        SafeMode::get()->setHackedLoad();

    if (Client::GetModuleEnabled("show-triggers"))
        SafeMode::get()->setHackedLoad();

    return true;
}

void SafePlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    SafeMode::get()->resetOnNewAttempt();
}

void SafePlayLayer::levelComplete()
{
    auto kick = SafeMode::get()->shouldKickFromLevel();

    if (kick)
    {
        return this->onQuit();
    }

    PlayLayer::levelComplete();
}

void SafePlayLayer::showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5)
{
    if (!SafeMode::get()->shouldKickFromLevel())
        PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
}

void SafeGJGameLevel::savePercentage(int p0, bool p1, int p2, int p3, bool p4)
{
    if (!SafeMode::get()->shouldKickFromLevel())
        GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
}