/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

class $modify (GameManager)
{
    bool getUGV(char const* key)
    {
        auto ret = GameManager::getUGV(key);

        if (std::string(key) == "31")
            ret = false;

        if (std::string(key) == "12")
            ret = false;

        log::info("key: {}, ret: {}", key, ret);

        return ret;
    }
};*/