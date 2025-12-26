#pragma once

#include "../../Client/CategoryNode.hpp"

class LanguageNode : public CategoryNode
{
    public:
        CREATE_FUNC(LanguageNode)

        void onTest1(CCObject* sender);
        void onTest2(CCObject* sender);

        bool init();
};

SUBMIT_CATEGORY("Config/Language", LanguageNode)