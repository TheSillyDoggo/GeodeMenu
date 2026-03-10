#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNodeUI : public CategoryNode
{
    protected:
        ~LanguageNodeUI();

    public:
        CREATE_FUNC(LanguageNodeUI)

        void onContribute(CCObject* sender);
        void onSettings(CCObject* sender);

        virtual void updateUI();

        bool init();
};

SUBMIT_CATEGORY("Config/Language", LanguageNodeUI)