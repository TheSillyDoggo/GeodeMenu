#ifdef GEODE_IS_WINDOWS

#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* part = nullptr;

class $modify (CCParticleSystem)
{
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool idk)
    {
        if (!part)
            part = Client::GetModule("no-particles");
        
        return CCParticleSystem::initWithTotalParticles(part->enabled ? 0 : numberOfParticles, idk);
    }

    virtual void update(float dt)
    {
        if (!part)
            part = Client::GetModule("no-particles");
        
        if (!part->enabled)
            CCParticleSystem::update(dt);
        else
            this->setScale(0);
    }

    virtual void setTotalParticles(unsigned int tp)
    {
        if (!part)
            part = Client::GetModule("no-particles");
        
        CCParticleSystem::setTotalParticles(part->enabled ? 0 : tp);
    }
};

class $modify (CCParticleSystemQuad)
{
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool idk)
    {
        if (!part)
            part = Client::GetModule("no-particles");
        
        return CCParticleSystemQuad::initWithTotalParticles(part->enabled ? 0 : numberOfParticles, idk);
    }

    virtual void setTotalParticles(unsigned int tp)
    {
        if (!part)
            part = Client::GetModule("no-particles");
        
        CCParticleSystemQuad::setTotalParticles(part->enabled ? 0 : tp);
    }
};

#endif