#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../../SafeMode/SafeMode.hpp"

using namespace geode::prelude;

class TempCheatIndicator : public Module
{
    public:
        MODULE_SETUP(TempCheatIndicator)
        {
            setName("TEMP cheat indicator");
            setID("temp-cheat-indicator");
            setCategory("Labels");
            setDescription("Temporary cheat indicator until labels have been added back");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(TempCheatIndicator);

class $modify (PlayLayer)
{
    struct Fields
    {
        CCLayerColor* colour = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        m_fields->colour = CCLayerColor::create(ccc4(255, 255, 255, 255), 6, 6);
        m_fields->colour->ignoreAnchorPointForPosition(false);
        m_fields->colour->setAnchorPoint(ccp(0, 1));
        m_fields->colour->setPosition(ccp(2, CCDirector::get()->getWinSize().height - 2));
        m_fields->colour->setZOrder(6980085);

        #ifdef GEODE_IS_MOBILE
        m_fields->colour->setContentSize(ccp(14, 14));
        #endif

        this->addChild(m_fields->colour);
        return true;
    }

    virtual void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        m_fields->colour->setColor(SafeMode::get()->getIndicatorColour());
        m_fields->colour->setVisible(TempCheatIndicator::get()->getRealEnabled());
    }
};