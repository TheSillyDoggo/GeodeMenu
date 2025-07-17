#include "../../Client/Module.hpp"
#include <Geode/modify/ShareLevelSettingsLayer.hpp>

using namespace geode::prelude;

class DisableCopying : public Module
{
    public:
        MODULE_SETUP(DisableCopying)
        {
            setName("Disable Copy");
            setID("disable-copying");
            setCategory("Creator");
            setDescription("Allows you to put disable copying and put passwords on levels, like in older versions");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(DisableCopying);

class $modify (ShareLevelSettingsLayer)
{
    bool init(GJGameLevel* p0)
    {
        if (!ShareLevelSettingsLayer::init(p0))
            return false;

        if (DisableCopying::get()->getRealEnabled())
        {
            // auto 
        }

        return true;
    }
};