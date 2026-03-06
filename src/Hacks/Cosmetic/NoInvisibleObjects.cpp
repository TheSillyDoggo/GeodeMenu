#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class NoInvisibleObjects : public Module
{
    public:
        MODULE_SETUP(NoInvisibleObjects)
        {
            setID("no-invisible-objects");
            setCategory("Cosmetic");
            setSafeModeTrigger(SafeModeTrigger::Attempt);

            #if GEODE_COMP_GD_VERSION <= 22074
            setDisabledMessage("Unsupported in GD 2.2074")
            setDisabled(true);
            #endif
        }
};

SUBMIT_HACK(NoInvisibleObjects);

#if GEODE_COMP_GD_VERSION <= 22074

class $modify (PlayLayer)
{
    void updateInvisibleBlock(GameObject* object, float rightFadeBound, float leftFadeBound, float rightFadeWidth, float leftFadeWidth, cocos2d::ccColor3B const& lbgColor)
    {
        if (NoInvisibleObjects::get()->getRealEnabled())
        {
            object->m_areaOpacityRelated = 255;
            return;
        }

        PlayLayer::updateInvisibleBlock(object, rightFadeBound, leftFadeBound, rightFadeWidth, leftFadeWidth, lbgColor);
    }
};

#endif