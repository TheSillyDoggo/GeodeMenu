#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

enum CategoryTabType
{
    Text,
    Image,
};

enum CategorySelectionType
{
    Deselected,
    Hovered,
    Selected,
};

class CategoryTabSprite : public CCNode
{
    public:
        CCScale9Sprite* outline = nullptr;
        CCScale9Sprite* background = nullptr;
        AdvLabelBMFont* label = nullptr;
        CCSprite* sprite = nullptr;

        static CategoryTabSprite* create(CategoryTabType type, std::string name = "", std::string icon = "");

        bool init(CategoryTabType type, std::string name, std::string icon);

        void updateSelection(CategorySelectionType type);

        virtual void setContentSize(const CCSize& contentSize);
};