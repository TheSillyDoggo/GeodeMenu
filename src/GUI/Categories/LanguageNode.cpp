#include "LanguageNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../BetterButtonSprite.hpp"

bool LanguageNode::init()
{
    if (!CategoryNode::init())
        return false;

    auto lbl = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("ui/coming-soon"), "bigFont.fnt");
    lbl->setScale(0.5f);

    auto menu = CCMenu::create();
    auto test1 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(ccp(110, 30), "DEV: Load English", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(LanguageNode::onTest1));
    auto test2 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(ccp(110, 30), "DEV: Load Japanese", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(LanguageNode::onTest2));
    test2->setPositionY(-40);

    menu->addChild(test1);
    menu->addChild(test2);

    this->addChildAtPosition(lbl, Anchor::Center);
    this->addChildAtPosition(menu, Anchor::Center, ccp(0, -50));

    return true;
}

void LanguageNode::onTest1(CCObject* sender)
{
    LocalisationManager::get()->switchLocalisationWithUI("en-AU.json");
}

void LanguageNode::onTest2(CCObject* sender)
{
    LocalisationManager::get()->switchLocalisationWithUI("ja-JP.json");
}