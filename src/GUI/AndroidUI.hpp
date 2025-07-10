#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "CategoryTabSprite.hpp"

using namespace geode::prelude;

class AndroidUI : public geode::Popup<>
{
    protected:
        static inline AndroidUI* instance = nullptr;

        BackgroundSprite* bg = nullptr;
        std::map<std::string, CategoryNode*> categories = {};
        std::map<std::string, CCMenuItemSpriteExtra*> categoryBtns = {};
        std::map<std::string, CategoryTabSprite*> categorySprs = {};
        std::string selectedCategory = "Level";

        ~AndroidUI();

    public:
        static AndroidUI* create();
        static AndroidUI* addToScene();
        static AndroidUI* get();

        void onSelectTab(CCObject* sender);

        void populateModules();
        void populateTabs();
        void updateTabs();

        void close();
        virtual bool setup();
};