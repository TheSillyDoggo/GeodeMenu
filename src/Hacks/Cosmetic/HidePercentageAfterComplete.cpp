#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class HidePercentAfterComplete : public Module
{
    public:
        MODULE_SETUP(HidePercentAfterComplete)
        {
            setName("Hide Percentage on Complete");
            setID("hide-percent-after-complete");
            setCategory("Cosmetic");
            setDescription("Fades out the progress bar and percentage label after a level is completed");
        }
};

SUBMIT_HACK(HidePercentAfterComplete);

class $modify (PlayLayer)
{
    void levelComplete()
    {
        PlayLayer::levelComplete();

        if (HidePercentAfterComplete::get()->getRealEnabled())
        {
            if (m_percentageLabel)
                m_percentageLabel->runAction(CCFadeTo::create(0.5f, 0));

            if (m_progressBar)
                m_progressBar->runAction(CCFadeTo::create(0.5f, 0));

            if (m_progressFill)
                m_progressFill->runAction(CCFadeTo::create(0.5f, 0));
        }
    }
};

class $modify (EndLevelLayer)
{
    void onReplay(cocos2d::CCObject* sender)
    {
        EndLevelLayer::onReplay(sender);

        if (auto pl = PlayLayer::get())
        {
            if (pl->m_percentageLabel)
                pl->m_percentageLabel->runAction(CCFadeTo::create(0.5f, 255));

            if (pl->m_progressBar)
                pl->m_progressBar->runAction(CCFadeTo::create(0.5f, 255));

            if (pl->m_progressFill)
                pl->m_progressFill->runAction(CCFadeTo::create(0.5f, 255));
        }
    }
};