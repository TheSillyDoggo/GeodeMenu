#include "../../Client/Module.hpp"
#include <Geode/modify/ShaderLayer.hpp>

using namespace geode::prelude;

class NoShaders : public Module
{
    public:
        MODULE_SETUP(NoShaders)
        {
            setName("No Shaders");
            setID("no-shaders");
            setCategory("Cosmetic");
            setDescription("Disables shaders");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(NoShaders);

class $modify (ShaderLayer)
{
    virtual void visit()
    {
        if (NoShaders::get()->getRealEnabled())
            return CCNode::visit();

        ShaderLayer::visit();
    }
};