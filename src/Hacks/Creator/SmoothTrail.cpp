#include "../../Client/Module.hpp"
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class SmoothTrail : public Module
{
    public:
        MODULE_SETUP(SmoothTrail)
        {
            setName("Smooth Editor Trail");
            setID("smooth-editor-trail");
            setCategory("Creator");
            setDescription("Updates the editor trail at your screen refresh rate instead of 30 fps");
        }
};

SUBMIT_HACK(SmoothTrail);

class $modify (LevelEditorLayer)
{
    virtual void postUpdate(float dt)
    {
        if (SmoothTrail::get()->getRealEnabled())
            m_trailTimer = 69420.42069f;

        LevelEditorLayer::postUpdate(dt);
    }
};