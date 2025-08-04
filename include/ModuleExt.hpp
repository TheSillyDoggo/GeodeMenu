#pragma once

#include <Geode/Geode.hpp>
#include "../src/Client/Module.hpp"

namespace QOLMod
{
    class AddModuleEvent : public geode::Event
    {
        public:
            Module* mod;

            AddModuleEvent(Module* mod)
            {
                this->mod = mod;
            }
    };
};