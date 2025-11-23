#pragma once

#include "../../Client/CategoryNode.hpp"

class SearchShowOptions : public Module
{
    public:
        MODULE_SETUP(SearchShowOptions)
        {
            setName("Show options");
            setID("search/show-options");
            setCategory("");
            setDescription("Shows mod options in the search results");
            setDefaultEnabled(true);
        }

        virtual void onToggle();
};

class SearchNode : public CategoryNode, public TextInputDelegate
{
    protected:
        TextInput* textInput = nullptr;
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