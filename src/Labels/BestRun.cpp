#include "BestRun.hpp"

void BestPlayLayer::resetLevel()
{
    if (m_fields->ignoreBest)
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
}

std::string BestPlayLayer::getRunString()
{
    if (m_fields->bestLength == 0)
        return "Best Run: None";

    return fmt::format("Best Run: {} - {}%", as<int>(m_fields->bestFrom), as<int>(m_fields->bestTo));
}