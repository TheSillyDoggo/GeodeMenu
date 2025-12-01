#pragma once

#include "../../Client/CategoryNode.hpp"

class LabelsNode : public CategoryNode
{
    public:
        CREATE_FUNC(LabelsNode)

        bool init();
};

SUBMIT_CATEGORY("Labels", LabelsNode)
SUBMIT_CATEGORY("Profiles", LabelsNode)
SUBMIT_CATEGORY("Config/Language", LabelsNode)