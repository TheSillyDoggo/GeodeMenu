#include "RunInfo.hpp"

void RunPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->fromPercent = getCurrentPercent();
}