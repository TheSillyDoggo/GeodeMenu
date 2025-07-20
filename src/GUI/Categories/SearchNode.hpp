#pragma once

#include "../../Client/CategoryNode.hpp"

class SearchNode : public CategoryNode, public TextInputDelegate
{
    protected:
        TextInput* textInput = nullptr;
        CCMenu* errorMenu = nullptr;
        CCLabelBMFont* error = nullptr;
    
    public:
        CREATE_FUNC(SearchNode)

        virtual void textChanged(CCTextInputNode* input);
        void onJoinDiscord(CCObject* sender);

        bool init();
};

SUBMIT_CATEGORY("Search", SearchNode)