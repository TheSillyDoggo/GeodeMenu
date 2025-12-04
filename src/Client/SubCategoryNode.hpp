#pragma once

#include "CategoryNode.hpp"
#include "../GUI/CategoryTabSprite.hpp"

class SubCategoryNode : public CategoryNode
{
    protected:
        CCMenu* selectSubMenu = nullptr;
        std::unordered_map<std::string, CategoryTabSprite*> normalSprs = {};
        std::unordered_map<std::string, CCMenuItemSpriteExtra*> normalBtns = {};
        std::unordered_map<std::string, CategoryNode*> categoryNodes = {};
        static inline std::unordered_map<std::string, std::string> selectedSub = {};
    
    public:
        CREATE_FUNC(SubCategoryNode);
        
        void onSelectSub(CCObject* sender);
        void addModule(Module* mod, std::string subCategory);

        void addSubCategory(std::string name);
        virtual void updateUI();

        bool init();
};