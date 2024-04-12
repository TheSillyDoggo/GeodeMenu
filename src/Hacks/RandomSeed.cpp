#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* randMod = nullptr;

/*
static void fast_srand(unsigned long seed) {
    *(int*)((char*)geode::base::get() + 0x4f24a8) = seed;
}
static TodoReturn gen_random(int);		static TodoReturn gen_random(int);
static TodoReturn getDropActionWDelay(float, float, float, cocos2d::CCNode*, cocos2d::SEL_CallFunc);		static TodoReturn getDropActionWDelay(float, float, float, cocos2d::CCNode*, cocos2d::SEL_CallFunc);
static TodoReturn getDropActionWEnd(float, float, float, cocos2d::CCAction*, float);		static TodoReturn getDropActionWEnd(float, float, float, cocos2d::CCAction*, float);
static TodoReturn getEasedAction(cocos2d::CCActionInterval*, int, float);		static TodoReturn getEasedAction(cocos2d::CCActionInterval*, int, float);
static TodoReturn getEasedValue(float, int, float);		static TodoReturn getEasedValue(float, int, float);
static TodoReturn getfast_srand();		static int getfast_srand() {
        return *(int*)((char*)geode::base::get() + 0x4f24a8); // global :(
}

*/

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

            #ifdef GEODE_IS_WINDOWS
            *(int*)((char*)geode::base::get() + 0x4f24a8) = seed;
            #else
            GameToolbox::fast_srand(seed);
            #endif
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

            #ifdef GEODE_IS_WINDOWS
            *(int*)((char*)geode::base::get() + 0x4f24a8) = seed;
            #else
            GameToolbox::fast_srand(seed);
            #endif
        }
    }
};