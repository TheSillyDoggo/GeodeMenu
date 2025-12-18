#include "BestRun.hpp"

void BestPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->fromPercent = getCurrentPercent();
}

void BestPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1)
{
    if (p1 == m_anticheatSpike)
        return PlayLayer::destroyPlayer(p0, p1);

    m_fields->lastPercent = getCurrentPercent();
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
    return "Not implemented";
}

std::string BestPlayLayer::getRunString()
{
    return "Not implemented";
}