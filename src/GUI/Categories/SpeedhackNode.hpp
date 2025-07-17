#pragma once

#include "../../Client/CategoryNode.hpp"

class SpeedhackNode : public CategoryNode
{
    public:
        CREATE_FUNC(SpeedhackNode)

        bool init();
};

SUBMIT_CATEGORY("Speedhack", SpeedhackNode)
// temp
SUBMIT_CATEGORY("Labels", SpeedhackNode)
SUBMIT_CATEGORY("Creator", SpeedhackNode)