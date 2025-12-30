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

    auto inp = BetterInputNode::create(120, "Placeholder Text"); // 私はかわいい猫が大好き

    this->addChildAtPosition(lbl, Anchor::Center);
    this->addChildAtPosition(inp, Anchor::Center, ccp(0, -60));
    return true;
}