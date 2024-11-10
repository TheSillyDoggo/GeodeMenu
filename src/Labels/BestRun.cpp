#include "BestRun.hpp"

Module* _accurateMod;

/*void BestPlayLayer::resetLevel()
{
    if (m_fields->ignoreBest || (m_startPosObject && getCurrentPercent() == 0))
        return PlayLayer::resetLevel();

    m_fields->toPercent = getCurrentPercent();

    auto length = m_fields->toPercent - m_fields->fromPercent;

    if (length > m_fields->bestLength)
    {
        m_fields->bestLength = length;
        m_fields->bestFrom = m_fields->fromPercent;
        m_fields->bestTo = m_fields->toPercent;
    }

    PlayLayer::resetLevel();
    m_fields->fromPercent = getCurrentPercent();
}*/

void BestPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->fromPercent = getCurrentPercent();
}

void BestPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1)
{
    if (p1 == m_anticheatSpike)
        return PlayLayer::destroyPlayer(p0, p1);

    m_fields->toPercent = getCurrentPercent();

    auto length = m_fields->toPercent - m_fields->fromPercent;

    if (length > m_fields->bestLength)
    {
        m_fields->bestLength = length;
        m_fields->bestFrom = m_fields->fromPercent;
        m_fields->bestTo = m_fields->toPercent;
    }

    PlayLayer::destroyPlayer(p0, p1);
}

std::string BestPlayLayer::getRoundedString(float f)
{
    int places = 0;

    if (!_accurateMod)
        _accurateMod = Client::GetModule("accurate-percentage");

    if (Client::GetModuleEnabled("best-run-decimals") && _accurateMod)
    {
        places = as<InputModule*>(_accurateMod->options[0])->getIntValue();

        if (!_accurateMod->enabled)
            places = GameManager::sharedState()->getGameVariable("0126") ? 2 : 0;
    }

    if (places == 0)
        return fmt::format("{}", as<int>(f));
    
    return utils::numToString<float>(f, places);
}

std::string BestPlayLayer::getRunString()
{
    auto fields = m_fields.self();

    if (fields->bestLength == 0)
        return "Best Run: None";

    if (!Client::GetModuleEnabled("best-run-always-show-from") && (as<int>(fields->fromPercent) == 0))
        return fmt::format("Best Run: {}%", getRoundedString(fields->bestTo));

    return fmt::format("Best Run: {} - {}%", getRoundedString(fields->bestFrom), getRoundedString(fields->bestTo));
}