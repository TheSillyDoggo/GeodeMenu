#include "Hooks.hpp"
#include "RangeActivationController.hpp"

using namespace qolmod;
using namespace geode::prelude;

bool RangesPLHook::init(GJGameLevel* level, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, dontCreateObjects))
        return false;

    RangeActivationController::get()->reset(this);
    return true;
}

void RangesBGLHook::processQueuedButtons(float dt, bool clearInputQueue)
{
    GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);

    if (auto pl = typeinfo_cast<PlayLayer*>(this))
    {
        RangeActivationController::get()->updateForPercent(pl->getCurrentPercent(), this);
    }
}