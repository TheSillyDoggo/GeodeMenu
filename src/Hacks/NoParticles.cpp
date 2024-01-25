#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCParticleSystem)
{
    virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool idk)
    {
        return CCParticleSystem::initWithTotalParticles(Client::GetModuleEnabled("no-particles") ? 0 : numberOfParticles, idk);
    }

    virtual void update(float dt)
    {
        if (!Client::GetModuleEnabled("no-particles"))
            CCParticleSystem::update(dt);
        else
            this->setScale(0);
    }

    virtual void setTotalParticles(unsigned int tp)
    {
        CCParticleSystem::setTotalParticles(Client::GetModuleEnabled("no-particles") ? 0 : tp);
    }
};

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