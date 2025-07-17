#include "SpeedhackNode.hpp"

bool SpeedhackNode::init()
{
    if (!CategoryNode::init())
        return false;

    this->addChildAtPosition(CCLabelBMFont::create("Coming soon", "bigFont.fnt"), Anchor::Center);

    return true;
}