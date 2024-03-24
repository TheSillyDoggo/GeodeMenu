#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* part = nullptr;

void myParticleUpdate(CCParticleSystem* ins, float dt)
{
    if (!part)
            part = Client::GetModule("no-particles");
    
    ins->update(dt);

    if (part->enabled)
        ins->setScale(0);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCParticleSystem::update)
        ),
        &myParticleUpdate,
        "cocos2d::CCParticleSystem::update",
        tulip::hook::TulipConvention::Thiscall
    );
}