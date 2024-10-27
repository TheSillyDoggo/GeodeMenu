#include "UnspeedhackedAction.hpp"
#include "CCDirectorExt.hpp"

bool UnspeedhackedAction::init(CCAction* action)
{
    this->action = action;

    return true;
}

void UnspeedhackedAction::step(float dt)
{
    dt = as<CCDirectorExt*>(CCDirector::get())->getUnmodifiedDelta();
    
    action->step(dt);
}

void UnspeedhackedAction::stop()
{
    action->stop();
    CCAction::stop();
}

bool UnspeedhackedAction::isDone()
{
    return action->isDone();
}

void UnspeedhackedAction::startWithTarget(CCNode *pTarget)
{
    CCAction::startWithTarget(pTarget);
    action->startWithTarget(pTarget);
}

UnspeedhackedAction* UnspeedhackedAction::create(CCAction* action)
{
    auto pRet = new UnspeedhackedAction();

    if (pRet && pRet->init(action))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}