#include "Hooks.hpp"
#include "Speedhack.hpp"
#include "../../Utils/ColourUtils.hpp"
#include "../../Labels/LabelManager.hpp"

Mod* cbf = nullptr;

bool CBFCheckMenuLayer::init()
{
    return MenuLayer::init();
    cbf = Loader::get()->getLoadedMod("syzzi.click_between_frames");

    return MenuLayer::init();
}

void SpeedhackScheduler::update(float dt)
{
    Speedhack::get()->realDeltatime = dt;
    ColourUtils::get()->update(dt);
    LabelManager::get()->update(dt);

    float value = Speedhack::get()->getRealValue();

    static float lastPitch = 0;
    float pi = (Speedhack::get()->getMusicEnabled() && Speedhack::get()->gameplayOnlyCheck()) ? value : 1;

    if (lastPitch != pi)
    {
        Speedhack::get()->getMasterChannel()->setPitch(pi);

        lastPitch = pi;
    }

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

bool isNodeUnpausable(CCNode* node)
{
    if (!node)
        return false;

    if (node->getUserFlag("unpausable"_spr))
        return true;

    if (node->getParent())
        return isNodeUnpausable(node->getParent());

    return false;
}

void SpeedhackScheduler::pauseTarget(CCObject *pTarget)
{
    if (auto node = typeinfo_cast<CCNode*>(pTarget))
    {
        if (isNodeUnpausable(node))
            return;
    }

    CCScheduler::pauseTarget(pTarget);
}

void SpeedhackBaseGameLayer::update(float dt)
{
    if (Speedhack::get()->getGameplayEnabled())
    {
        dt *= Speedhack::get()->getRealValue();
    }

    GJBaseGameLayer::update(dt);
}