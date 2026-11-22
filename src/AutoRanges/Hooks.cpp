#include "Hooks.hpp"
#include "RangeActivationController.hpp"

using namespace qolmod;
using namespace geode::prelude;

bool RangesBGLHook::init()
{
    if (!GJBaseGameLayer::init())
        return false;

    RangeManager::get()->getConfig("show-hitboxes_global")->m_ranges.addRange(5, 15, true);

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