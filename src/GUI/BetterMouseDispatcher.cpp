#include "BetterMouseDispatcher.hpp"

#ifndef GEODE_IS_IOS

bool BetterMouseDispatcher::dispatchScrollMSG(float x, float y)
{
    auto pRet = CCMouseDispatcher::dispatchScrollMSG(x, y);

    for (auto dele : MouseDispatcher::betterMouseDispatcherDelegates)
    {
        dele->scrollWheel(x, y);
    }

    return pRet;
}

#endif