#pragma once

#include "../Client/ButtonModule.hpp"

class CreateCustomReward : public ButtonModule
{
    public:
        MODULE_SETUP(CreateCustomReward)
        {
            setID("create-custom-reward");
            setCategory("Universal");

            setPriority(sortPriority + 1);
        }

        virtual void onClick();
};

SUBMIT_HACK(CreateCustomReward);