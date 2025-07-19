#pragma once

#include "../../Client/CategoryNode.hpp"

class SearchNode : public CategoryNode, public TextInputDelegate
{
    protected:
        TextInput* textInput = nullptr;
        CCLabelBMFont* error = nullptr;
    
    public:
        CREATE_FUNC(SearchNode)

        virtual void textChanged(CCTextInputNode* input);

        bool init();
};

SUBMIT_CATEGORY("Search", SearchNode)