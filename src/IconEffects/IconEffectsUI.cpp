#include "IconEffectsUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterInputNode.hpp"

bool IconEffectsUI::init()
{
    if (!CategoryNode::init())
        return false;

    auto lbl = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("ui/coming-soon"), "bigFont.fnt");
    lbl->setScale(0.5f);

    this->addChildAtPosition(lbl, Anchor::Center);
    return true;
}