#pragma once

#include <Geode/Geode.hpp>

struct KeyStruct
{
    public:
        // Modifiers
        bool control;
        bool alt;
        bool shift;
        bool command;

        cocos2d::enumKeyCodes key = cocos2d::enumKeyCodes::KEY_Unknown;
        bool canRepeat = true;

        static KeyStruct loadFromModule(std::string id);
        void saveToModule(std::string id);
};