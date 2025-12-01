#include "ConfigNode.hpp"

bool ConfigNode::init()
{
    if (!SubCategoryNode::init())
        return false;

    this->setID("Config");

    addSubCategory("Menu");
    addSubCategory("Button");
    addSubCategory("Theme");
    addSubCategory("Language");

    return true;
}