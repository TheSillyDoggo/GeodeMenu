#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class BestInPercentage : public Module
{
    public:
        MODULE_SETUP(BestInPercentage)
        {
            setName("Best In Percentage");
            setID("best-in-percentage");
            setCategory("Level");
            setDescription("Best in percentage help :c");
        }
};

SUBMIT_HACK(BestInPercentage);

class $modify (PlayLayer)
{
    void updateProgressbar()
    {
        PlayLayer::updateProgressbar();

        if (BestInPercentage::get()->getRealEnabled())
        {
            if (m_percentageLabel && !m_level->isPlatformer())
            {
                m_percentageLabel->setString(fmt::format("{} / {}%", m_percentageLabel->getString(), m_isPracticeMode ? m_level->m_practicePercent : m_level->m_normalPercent.value()).c_str());
            }
        }
    }
};