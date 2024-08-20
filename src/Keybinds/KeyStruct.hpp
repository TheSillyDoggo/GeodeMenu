#pragma once

#include <Geode/Geode.hpp>

struct KeyStruct
{
    public:
        // Modifiers
        bool control = false;
        bool alt = false;
        bool shift = false;
        bool command = false;

        cocos2d::enumKeyCodes key = cocos2d::enumKeyCodes::KEY_Unknown;
        bool canRepeat = true;

        static KeyStruct loadFromModule(std::string id);
        void saveToModule(std::string id);

        std::string toString();
};