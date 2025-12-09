#include "../../Client/InputModule.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class AccuratePercentage : public Module
{
    public:
        MODULE_SETUP(AccuratePercentage)
        {
            setName("Accurate Percentage");
            setID("accurate-percentage");
            setCategory("Level");
            setDescription("Allows you to have any amount of decimal places in your percentage text");
        }
};

class AccuratePercentagePlaces : public InputModule
{
    public:
        MODULE_SETUP(AccuratePercentagePlaces)
        {
            setName("Places");
            setID("accurate-percentage/places");
            setDescription("");

            setDefaultString("2");
            setPlaceholderString("Places");

            setStringFilter("1234567890");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(AccuratePercentage);
SUBMIT_OPTION(AccuratePercentage, AccuratePercentagePlaces);

class $modify (PlayLayer)
{
    void updateProgressbar()
    {
        PlayLayer::updateProgressbar();

        if (!(AccuratePercentage::get()->getRealEnabled()))
            return;

        if (m_percentageLabel)
        {
            m_percentageLabel->setString(fmt::format("{}%", utils::numToString<float>(getCurrentPercent(), AccuratePercentagePlaces::get()->getStringInt())).c_str());
        }
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("PlayLayer::updateProgressbar", Priority::First);
    }
};