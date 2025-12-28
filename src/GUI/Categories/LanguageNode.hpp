#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNodeUI : public CategoryNode
{
    public:
        CREATE_FUNC(LanguageNodeUI)

        virtual void updateUI();

        bool init();
};

SUBMIT_CATEGORY("Config/Language", LanguageNodeUI)