#pragma once

#include "../../Client/CategoryNode.hpp"
#include <ColourModule.hpp>
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

class SearchOptionsColour : public ColourModule
{
    public:
        MODULE_SETUP(SearchOptionsColour)
        {
            setID("search/options-colour");
            setDefaultConfig({ ccc3(255, 175, 0) });
        }
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

        void updateColour(float);

        void onJoinDiscord(CCObject* sender);
        void onFilter(CCObject* sender);

        bool init();
};

SUBMIT_CATEGORY("Search", SearchNode)