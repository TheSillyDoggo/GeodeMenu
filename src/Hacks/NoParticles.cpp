#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCParticleSystemQuad)
{
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool idk)
    {
        return CCParticleSystemQuad::initWithTotalParticles(Client::GetModuleEnabled("no-particles") ? 0 : numberOfParticles, idk);
    }

    virtual void setTotalParticles(unsigned int tp)
    {
        CCParticleSystemQuad::setTotalParticles(Client::GetModuleEnabled("no-particles") ? 0 : tp);
    }
};