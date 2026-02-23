#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNodeUI : public CategoryNode
{
    public:
        CREATE_FUNC(LanguageNodeUI)

        virtual void updateUI();

        void onContribute(CCObject* sender);
        bool init();
};

SUBMIT_CATEGORY("Config/Language", LanguageNodeUI)