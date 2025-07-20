#include "Hooks.hpp"
#include "Speedhack.hpp"

Mod* cbf = nullptr;

bool CBFCheckMenuLayer::init()
{
    cbf = Loader::get()->getLoadedMod("syzzi.click_between_frames");

    return MenuLayer::init();
}

void SpeedhackScheduler::update(float dt)
{
    float value = Speedhack::get()->getRealValue();

    Speedhack::get()->getMasterChannel()->setPitch((Speedhack::get()->getMusicEnabled() && Speedhack::get()->gameplayOnlyCheck()) ? value : 1);

    if (Speedhack::get()->getGameplayEnabled())
    {
        CCScheduler::update(dt);
        
        return;
    }

    if (cbf)
    {
        auto director = CCDirector::get();

        director->m_fActualDeltaTime *= value;
        director->m_fDeltaTime *= value;
    }

    CCScheduler::update(dt * value);
}

void SpeedhackBaseGameLayer::update(float dt)
{
    if (Speedhack::get()->getGameplayEnabled())
    {
        dt *= Speedhack::get()->getRealValue();
    }

    GJBaseGameLayer::update(dt);
}