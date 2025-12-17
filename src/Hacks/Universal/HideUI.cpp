#include "../../Client/Module.hpp"
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

class HideUI : public Module
{
    public:
        MODULE_SETUP(HideUI)
        {
            setName("Hide UI");
            setID("hide-all-ui");
            setCategory("Universal");
            setDescription("Hides all of the UI ingame and in the editor (can still be interated with by touching where it is).");
        }
};

// SUBMIT_HACK(HideUI);

class $modify (UILayer)
{
    
};