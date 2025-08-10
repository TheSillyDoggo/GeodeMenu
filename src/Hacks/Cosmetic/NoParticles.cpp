#include "../../Client/Module.hpp"
#include <Geode/modify/CCParticleSystemQuad.hpp>

using namespace geode::prelude;

class NoParticles : public Module
{
    public:
        MODULE_SETUP(NoParticles)
        {
            setName("No Particles");
            setID("no-particles");
            setCategory("Cosmetic");
            setDescription("Disables particles");
        }
};

class ParticlesInEditor : public Module
{
    public:
        MODULE_SETUP(ParticlesInEditor)
        {
            setName("Allow in Editor");
            setID("no-particles/particles-in-editor");
            setCategory("Cosmetic");
            setDescription("Allows particles to show in the editor");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(NoParticles);
SUBMIT_OPTION(NoParticles, ParticlesInEditor);

// TODO: This should be a hook in a module, but since thats broken i have to do this :c
class $modify (CCParticleSystemQuad)
{
    virtual void draw()
    {
        if (NoParticles::get()->getRealEnabled() ? (ParticlesInEditor::get()->getRealEnabled() ? LevelEditorLayer::get() : false) : true)
            CCParticleSystemQuad::draw();
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("cocos2d::CCParticleSystemQuad::draw", Priority::Last);
    }
};