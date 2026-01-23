#include "AccuratePercentage.hpp"
#include "BestInPercentageLabel.hpp"
#include "ClassicPercentage.hpp"
#include "../../Utils/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../../../Labels/BestRun.hpp"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    void updateProgressbar()
    {
        PlayLayer::updateProgressbar();
        
        auto utils = PlayLayerUtils::getUtils();

        if (m_percentageLabel && !m_level->isPlatformer())
        {
            auto str = utils::numToString<float>(utils->getCurrentPercentageAdv(), utils->getDecimalsToShow());
            std::string fromInfo;

            if (utils->getDecimalsToShow() == 0)
                str = utils::numToString<int>(utils->getCurrentPercentageAdv());

            if (auto runInfo = Loader::get()->getLoadedMod("mat.run-info"))
            {
                if (runInfo->getSettingValue<bool>("show-in-percentage"))
                {
                    int dc = runInfo->getSettingValue<int>("decimal-places");
                    auto from = reinterpret_cast<BestPlayLayer*>(this)->m_fields->fromPercent;

                    if (dc == 0)
                        fromInfo.append(utils::numToString<int>(from));
                    else
                        fromInfo.append(utils::numToString<float>(from, dc));
                    
                    fromInfo.append("-");
                }
            }

            m_percentageLabel->setString(fmt::format("{}%", str).c_str());

            if (BestInPercentage::get()->getRealEnabled())
                m_percentageLabel->setString(fmt::format("{}{} / {}%", fromInfo, m_percentageLabel->getString(), m_isPracticeMode ? m_level->m_practicePercent : m_level->m_normalPercent.value()).c_str());
        }
    }
};