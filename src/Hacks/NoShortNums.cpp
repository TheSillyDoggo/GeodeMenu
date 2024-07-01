#include <Geode/Geode.hpp>
#include <Geode/modify/GameToolbox.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameToolbox)
{
    static gd::string intToShortString(int num)
    {
        return Client::GetModuleEnabled("no-short-nums") ? fmt::format("{}", num) : GameToolbox::intToShortString(num);
    }
};