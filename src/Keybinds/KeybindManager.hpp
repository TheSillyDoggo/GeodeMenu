#pragma once

#include "KeybindStruct.hpp"

class KeybindManager
{
    protected:
        std::vector<KeybindStruct*> structs = {};

    public:
        static KeybindManager* get();

        void addStruct(KeybindStruct* str);
        KeybindStruct* getStruct(std::string id);

        bool processMSG(KeyState state);
};