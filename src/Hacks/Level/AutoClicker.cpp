/*#include "../../Client/Module.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

static double getTimestamp()
{
    static LARGE_INTEGER freq = []{
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        return f;
    }();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    // he uses LowPart for some reason?
    return static_cast<double>(counter.LowPart) / static_cast<double>(freq.LowPart);
}

float interval = 0.1f;
float clickFor = 0.05f;
float value = 0;
bool rep = true;

class $modify (GJBaseGameLayer)
{
    void processCommands(float dt, bool isHalfTick, bool isLastTick)
    {
        value += dt;

        if (value > interval)
        {
            value -= interval;

            handleButton(rep, 1, true);

            rep = !rep;
        }

        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }

    void resetLevelVariables()
    {
        GJBaseGameLayer::resetLevelVariables();
        rep = true;
        value = 0;
    }
};*/