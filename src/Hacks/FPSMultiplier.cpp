/*#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

int multiplier = 25;

class $modify (CCScheduler)
{
    virtual void update(float dt)
    {
        for (size_t i = 0; i < multiplier; i++)
        {
            CCScheduler::update(dt / multiplier);
        }
        
    }
};
*/
/*class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        for (size_t i = 0; i < multiplier; i++)
        {
            GJBaseGameLayer::update(p0 / multiplier);
        }
    }
};

class $modify (PlayerObject)
{
    virtual void update(float p0)
    {
        for (size_t i = 0; i < multiplier; i++)
        {
            PlayerObject::update(p0 / multiplier);
        }
    }
};*/