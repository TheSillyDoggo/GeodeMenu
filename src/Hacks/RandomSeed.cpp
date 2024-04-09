#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* randMod = nullptr;

class $modify (GJBaseGameLayer)
{
    void resetLevelVariables()
    {
        GJBaseGameLayer::resetLevelVariables();
        
        if(!randMod)
            randMod = Client::GetModule("rand-seed");

        if (randMod->enabled)
        {
            int seed = 69420;

            auto x = numFromString<int>(as<InputModule*>(randMod->options[0])->text);

            if (x.has_value())
                seed = x.value();

            GameToolbox::fast_srand(seed);
        }
    }
};

class $modify (PlayLayer)
{
    void resetLevel()
    {
        PlayLayer::resetLevel();

        if(!randMod)
            randMod = Client::GetModule("rand-seed");

        if (randMod->enabled)
        {
            int seed = 69420;

            auto x = numFromString<int>(as<InputModule*>(randMod->options[0])->text);

            if (x.has_value())
                seed = x.value();

            GameToolbox::fast_srand(seed);
        }
    }
};