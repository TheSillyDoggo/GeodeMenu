#include "AccuratePercentage.hpp"
#include "BestInPercentageLabel.hpp"
#include "ClassicPercentage.hpp"
#include "../../Utils/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify (PlayLayer)
{
    void updateProgressbar()
    {
        PlayLayer::updateProgressbar();

        auto utils = PlayLayerUtils::getUtils();

        if (m_percentageLabel && !m_level->isPlatformer())
        {
            m_percentageLabel->setString(fmt::format("{}%", utils::numToString<float>(utils->getCurrentPercentageAdv(), utils->getDecimalsToShow())).c_str());

            if (BestInPercentage::get()->getRealEnabled())
                m_percentageLabel->setString(fmt::format("{} / {}%", m_percentageLabel->getString(), m_isPracticeMode ? m_level->m_practicePercent : m_level->m_normalPercent.value()).c_str());
        }
    }
};