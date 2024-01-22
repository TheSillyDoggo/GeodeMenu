#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCScheduler)
{
    void update(float dt)
    {
        if (SpeedhackTop::instance)
        {
            if (Client::GetModuleEnabled("speedhack-enabled"))
            {
                float v = 1.0f;
                
                try
                {
                    v = std::stof(SpeedhackTop::instance->text);
                }
                catch(const std::exception& e)
                {
                    v = 1.0f;
                }

                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;
                
                CCScheduler::update(dt * v);
                return;
            }
        }
        
        CCScheduler::update(dt);
    }
};