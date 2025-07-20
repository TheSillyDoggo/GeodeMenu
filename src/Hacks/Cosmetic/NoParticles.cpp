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

SUBMIT_HACK(NoParticles);

// TODO: This should be a hook in a module, but since thats broken i have to do this :c
class $modify (CCParticleSystemQuad)
{
    virtual void draw()
    {
        if (!NoParticles::get()->getRealEnabled())
            CCParticleSystemQuad::draw();
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("cocos2d::CCParticleSystemQuad::draw", Priority::Last);
    }
};