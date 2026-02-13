#include "RealtimeAction.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"

RealtimeAction* RealtimeAction::create(CCAction* action)
{
    auto pRet = new RealtimeAction();

    if (pRet && pRet->init(action))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void RealtimeAction::startWithTarget(CCNode *pTarget)
{
    CCAction::startWithTarget(pTarget);
    action->startWithTarget(pTarget);
}

bool RealtimeAction::init(CCAction* action)
{
    this->action = action;

    return true;
}

void RealtimeAction::step(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    
    action->step(dt);
}

void RealtimeAction::stop()
{
    action->stop();
    CCAction::stop();
}

bool RealtimeAction::isDone()
{
    return action->isDone();
}