#pragma once

#include "../../Client/SubCategoryNode.hpp"

class ConfigNode : public SubCategoryNode
{
    public:
        CREATE_FUNC(ConfigNode)

        bool init();
};

SUBMIT_CATEGORY("Config", ConfigNode)