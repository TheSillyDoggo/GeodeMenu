#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNode : public CategoryNode
{
    public:
        CREATE_FUNC(LanguageNode)

        bool init();
};

SUBMIT_CATEGORY("Icon Effects", LanguageNode)
SUBMIT_CATEGORY("Profiles", LanguageNode)
SUBMIT_CATEGORY("Config/Language", LanguageNode)