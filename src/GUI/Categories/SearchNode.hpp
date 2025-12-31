#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../BetterInputNode.hpp"

class SearchShowOptions : public Module
{
    public:
        MODULE_SETUP(SearchShowOptions)
        {
            setID("search/show-options");
            setDefaultEnabled(true);
        }

        virtual void onToggle();
};

class SearchNode : public CategoryNode, public TextInputDelegate
{
    protected:
        BetterInputNode* textInput = nullptr;
        CCMenu* errorMenu = nullptr;
        CCLabelBMFont* error = nullptr;
        static inline SearchNode* instance = nullptr;
    
        ~SearchNode();

    public:
        CREATE_FUNC(SearchNode)
        static SearchNode* get();

        virtual void textChanged(CCTextInputNode* input);
        virtual void addModule(Module* module);

        void onJoinDiscord(CCObject* sender);
        void onFilter(CCObject* sender);

        bool init();
};

SUBMIT_CATEGORY("Search", SearchNode)