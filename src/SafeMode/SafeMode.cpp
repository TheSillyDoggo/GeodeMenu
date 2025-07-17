#include "SafeMode.hpp"
#include "Modules/AutoSafeMode.hpp"
#include "Modules/SafeMode.hpp"

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
}

void SafeMode::addMessage(SafeModeTrigger level, std::string message)
{
    log::debug("Cheated: {}, {}", (level == SafeModeTrigger::Attempt ? "Attempt" : "LevelLoad"), message);

    switch (level)
    {
        case SafeModeTrigger::Attempt:
            attemptReasons.push_back(message);
            break;

        case SafeModeTrigger::LevelLoad:
            levelLoadReasons.push_back(message);
            break;
    }
}

bool SafeMode::shouldDisableLevelProgress()
{
    if (AutoSafeMode::get()->getRealEnabled())
    {
        if (isLevelLoadCheated || isAttemptCheated)
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

    return ccc3(0, 255, 0);
}