#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"
#include "ModuleNode.hpp"
#include "../GUI/BetterScrollbar.hpp"

using namespace geode::prelude;

class CategoryNode : public CCMenu
{
    friend class AndroidUI;

    protected:
        static inline std::map<std::string, std::function<CategoryNode*()>> advCategories = {};
        std::unordered_map<Module*, ModuleNode*> modules = {};
        ScrollLayer* scroll = nullptr;
        BetterScrollbar* scrollbar = nullptr;
        CCScale9Sprite* bg = nullptr;
        bool alwaysShowScrollbar = false;
    
    public:
        static CategoryNode* create();
        static void addAdvanced(std::string name, std::function<CategoryNode*()> func);
        static CategoryNode* getNode(std::string category);

        virtual void addModule(Module* module);
        void removeModule(Module* module);
        void removeAll();
        
        virtual void updateUI();
        bool shouldScrollbarShow();

        virtual void setContentSize(const CCSize& contentSize);

        bool init();
};

#define SUBMIT_CATEGORY(name, func) \
$execute \
{ \
    CategoryNode::addAdvanced(name, []{ \
        return func::create(); \
    }); \
}