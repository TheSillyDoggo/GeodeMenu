#include "SafeMode.hpp"
#include "Modules/AutoSafeMode.hpp"
#include "Modules/SafeMode.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"

SafeMode* SafeMode::get()
{
    static SafeMode* instance = nullptr;

    if (!instance)
        instance = new SafeMode();

    return instance;
}

void SafeMode::onLevelLoad()
{
    isLevelLoadCheated = false;
    levelLoadReasons.clear();

    for (auto module : Module::getAll())
    {
        if (module->getSafeModeTrigger() == SafeModeTrigger::LevelLoad)
        {
            if (module->getRealEnabled())
            {
                isLevelLoadCheated = true;
                addMessage(module->getSafeModeTrigger(), fmt::format("<cc>{}</c> enabled on <co>level load</c>", module->getName()));
            }
        }
    }

    onLevelNewAttempt();
}

void SafeMode::onLevelNewAttempt()
{
    isAttemptCheated = false;
    attemptReasons.clear();

    for (auto module : Module::getAll())
    {
        if (module->getSafeModeTrigger() == SafeModeTrigger::Attempt)
        {
            if (module->getRealEnabled())
            {
                isAttemptCheated = true;
                addMessage(module->getSafeModeTrigger(), fmt::format("<cc>{}</c> enabled for <co>this attempt</c>", module->getName()));
            }
        }
    }

    onSpeedhackChanged();
}

void SafeMode::onModuleToggled(Module* mod)
{
    if (mod->getSafeModeTrigger() == SafeModeTrigger::Attempt)
    {
        isAttemptCheated = true;
        addMessage(mod->getSafeModeTrigger(), fmt::format("<cc>{}</c> enabled for <co>this attempt</c>", mod->getName()));
    }
}

void SafeMode::onSpeedhackChanged()
{
    if (Speedhack::get()->getRealValue() != 1.0f)
    {
        isAttemptCheated = true;
        addMessage(SafeModeTrigger::Attempt, "<cc>Speedhack</c> enabled for <co>this attempt</c>");
    }
}

void SafeMode::addMessage(SafeModeTrigger level, std::string message)
{
    switch (level)
    {
        case SafeModeTrigger::Attempt:
            if (std::find(attemptReasons.begin(), attemptReasons.end(), message) == attemptReasons.end())
                attemptReasons.push_back(message);
            break;

        case SafeModeTrigger::LevelLoad:
            levelLoadReasons.push_back(message);
            break;

        case SafeModeTrigger::None:
            break;

        case SafeModeTrigger::Custom:
            break;
    }
}

bool SafeMode::shouldDisableLevelProgress()
{
    if (AutoSafeMode::get()->getRealEnabled())
    {
        if (isLevelLoadCheated || isAttemptCheated)
            return true;

        if (PlayLayer::get() && PlayLayer::get()->m_startPosObject)
            return true;
    }

    if (ForceSafeMode::get()->getRealEnabled())
        return true;

    return false;
}

ccColor3B SafeMode::getIndicatorColour()
{
    if (ForceSafeMode::get()->getRealEnabled())
        return ccc3(255, 255, 0);

    if (isLevelLoadCheated || isAttemptCheated)
        return ccc3(255, 0, 0);

    if (PlayLayer::get() && PlayLayer::get()->m_startPosObject)
        return ccc3(0, 133, 55);

    return ccc3(0, 255, 0);
}

std::string SafeMode::getCombinedMessages()
{
    std::string str = "";

    for (auto reason : levelLoadReasons)
    {
        str.append(reason);
        str.append("\n");
    }

    for (auto reason : attemptReasons)
    {
        str.append(reason);
        str.append("\n");
    }

    if (PlayLayer::get() && PlayLayer::get()->m_startPosObject)
        str.append("<co>Played level</c> from a <cc>start position</c>\n");

    return str;
}