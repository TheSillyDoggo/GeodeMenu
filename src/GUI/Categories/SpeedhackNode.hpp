#pragma once

#include "../../Client/CategoryNode.hpp"

class SpeedhackNode : public CategoryNode
{
    public:
        CREATE_FUNC(SpeedhackNode)

        bool init();
};

SUBMIT_CATEGORY("Speedhack", SpeedhackNode)