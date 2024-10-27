#include "CCDirectorExt.hpp"

double __deltatime__;

float CCDirectorExt::getUnmodifiedDelta()
{
    return __deltatime__;
}

void CCSchedulerExt::update(float dt)
{
    __deltatime__ = dt;

    CCScheduler::update(dt);
}