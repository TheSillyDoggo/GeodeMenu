#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class UseColonThreeButton : public Module
{
    public:
        MODULE_SETUP(UseColonThreeButton)
        {
            setName("Use \":3\" button");
            setID("use-:3-button");
            setCategory("");
            setDescription("Changes the \">_\" button to \":3\"");
        }
};














SUBMIT_HACK(UseColonThreeButton);