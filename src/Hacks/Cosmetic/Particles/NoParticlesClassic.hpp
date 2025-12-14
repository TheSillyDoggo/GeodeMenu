#pragma once

#include "../../../Client/Module.hpp"

using namespace geode::prelude;

class NoParticlesClassic : public Module
{
    public:
        MODULE_SETUP(NoParticlesClassic)
        {
            setName("No Particles Classic");
            setID("no-particles");
            setCategory("Cosmetic");
            setDescription("Disables all particles in the game");
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

SUBMIT_HACK(NoParticlesClassic);
SUBMIT_OPTION(NoParticlesClassic, ParticlesInEditor);