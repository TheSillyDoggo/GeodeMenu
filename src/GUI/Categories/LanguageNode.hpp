#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNodeUI : public CategoryNode
{
    protected:
        ~LanguageNodeUI();

        virtual void updateUILayout();

    public:
        CREATE_FUNC(LanguageNodeUI)

        void onContribute(CCObject* sender);
        void onSettings(CCObject* sender);

        bool init();
};

SUBMIT_CATEGORY("Config/Language", LanguageNodeUI)