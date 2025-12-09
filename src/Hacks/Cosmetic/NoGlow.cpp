#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class NoGlow : public Module
{
    public:
        MODULE_SETUP(NoGlow)
        {
            setName("No Glow");
            setID("no-death");
            setCategory("Cosmetic");
            setDescription("Disables object glow.\nRequires re-entering the level to apply");
        }
};

SUBMIT_HACK(NoGlow);

class $modify (PlayLayer)
{
    void addObject(GameObject* object)
    {
        if (NoGlow::get()->getRealEnabled())
            object->m_hasNoGlow = true;

        PlayLayer::addObject(object);
    }
};