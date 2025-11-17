#include "LabelsNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

bool LabelsNode::init()
{
    if (!CategoryNode::init())
        return false;

    auto lbl = CCLabelBMFont::create("Work in progress!", "bigFont.fnt");
    lbl->setScale(0.5f);

    this->addChildAtPosition(lbl, Anchor::Center);

    return true;
}