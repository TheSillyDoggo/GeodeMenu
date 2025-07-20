#include "LabelsNode.hpp"

bool LabelsNode::init()
{
    if (!CategoryNode::init())
        return false;

    auto lbl = CCLabelBMFont::create("Coming soon", "bigFont.fnt");
    lbl->setScale(0.5f);

    this->addChildAtPosition(lbl, Anchor::Center);

    return true;
}