#include "RangeActivationController.hpp"
#include "Module.hpp"
#include "../SafeMode/SafeMode.hpp"
#include "NotificationManager.hpp"
#include "LocalisationManager.hpp"

using namespace qolmod;
using namespace geode::prelude;

RangeActivationController* RangeActivationController::get()
{
    static RangeActivationController instance;
    return &instance;
}

bool RangeActivationController::isInRange(Module* module, bool& enabledOut)
{
    auto& data = m_data[module];

    enabledOut = data.m_inRange && data.m_rangeEnable;
    return data.m_inRange;
}

void RangeActivationController::notifyRange(Module* module, float percent)
{
    auto str = LocalisationManager::get()->getLocalisedString(m_data[module].m_rangeEnable ? "ui/notification-entered-range-enable" : "ui/notification-entered-range-disable");
    str = utils::string::replace(str, "{mod}", module->getName());
    str = utils::string::replace(str, "{percent}", utils::numToString(percent, 2));

    NotificationManager::get()->notifyToast(str);
}

void RangeActivationController::updateForPercent(float percent, GJBaseGameLayer* layer)
{
    auto mgr = RangeManager::get();
    for (auto& mod : Module::getAll())
    {
        if (!mod) continue;
        auto& data = m_data[mod];

        data.m_lastFrameWasInRange = data.m_inRange;
        data.m_lastFrameWasEnabled = data.m_rangeEnable;
        auto beforeEn = mod->getRealEnabled();

        switch (mgr->getMode(mod))
        {
            case ModRangeMode::Disabled: {
                data.m_inRange = false;
                data.m_rangeEnable = false;
                break;
            }
            case ModRangeMode::PerLevel:
            case ModRangeMode::Global: {
                auto conf = mgr->getMode(mod) == ModRangeMode::PerLevel ? data.m_levelConf : data.m_globalConf;

                if (conf)
                {
                    data.m_rangeEnable = conf->m_ranges.getEnable(percent, false, &data.m_inRange);

                    if (data.m_inRange != data.m_lastFrameWasEnabled && conf->m_notify)
                    {
                        if (mod->getRealEnabled() != beforeEn)
                            notifyRange(mod, percent);
                    }
                }
                break;
            }
        }

        if (data.m_inRange && data.m_rangeEnable)
            SafeMode::get()->onModuleToggled(mod);
    }
}

void RangeActivationController::reset(GJBaseGameLayer* layer)
{
    auto mgr = RangeManager::get();
    m_data.clear();

    for (auto& mod : Module::getAll())
    {
        m_data[mod] = {
            .m_globalConf = mgr->getConfig(mgr->getKey(mod, nullptr)),
            .m_levelConf = mgr->getConfig(mgr->getKey(mod, layer->m_level)),
            .m_inRange = false,
            .m_rangeEnable = false,
            .m_lastFrameWasInRange = false,
            .m_lastFrameWasEnabled = false
        };
    }
}