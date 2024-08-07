#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* part = nullptr;

#ifdef GEODE_IS_IOS

class $modify (CCParticleSystem)
{
    virtual void update(float dt)
    {
        this->setScale(0);
    }

    QOLMOD_MOD_ALL_HOOKS("no-particles")
};

#else

void myParticleUpdate(CCParticleSystem* ins, float dt)
{
    ins->update(dt);
    ins->setScale(0);
}

$execute {
    auto hook = Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCParticleSystem::update)
        ),
        &myParticleUpdate,
        "cocos2d::CCParticleSystem::update",
        tulip::hook::TulipConvention::Thiscall
    ).unwrap();

    Loader::get()->queueInMainThread([hook]
    {
        auto modu = Client::GetModule("no-particles");
        modu->addHook(hook);
    });
}

#endif

#endif