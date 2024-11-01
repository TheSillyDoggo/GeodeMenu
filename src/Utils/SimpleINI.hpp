#pragma once

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

using namespace geode::prelude;

class SimpleINI
{
    private:
        std::unordered_map<std::string, std::string> headerKeys;

    public:
        static SimpleINI* createWithString(std::string ini);
        static SimpleINI* createWithFile(std::string file);

        bool init(std::string data);

        bool hasKey(std::string key);
        std::string getKeyValue(std::string key, std::string def);
        float getKeyValueFloat(std::string key, std::string def);
        int getKeyValueInt(std::string key, std::string def);
        
};