/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameManager)
{
    bool getUGV(char const* id)
    {
        //log::info("get ugv: {}", id);
        //log::info("expected result: {}", GameManager::getUGV(id));

        if (std::stoi(id) == 31)
        {
            //log::info("zulguroth");
            return false;
        }
        else
            return GameManager::getUGV(id);
    }

    bool getGameVariable(char const* id)
    {
        //0024
        //log::info("getting bool game var: {}", id);

        return GameManager::getGameVariable(id);
    }
};*/