#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum CategoryTabType
{
    Text,
    Favourite,
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
        CCScale9Sprite* outline;
        CCScale9Sprite* background;
        CCLabelBMFont* label;

        static CategoryTabSprite* create(CategoryTabType type, std::string name = "");

        bool init(CategoryTabType type, std::string name);

        void updateSelection(CategorySelectionType type);

        virtual void setContentSize(const CCSize& contentSize);
};