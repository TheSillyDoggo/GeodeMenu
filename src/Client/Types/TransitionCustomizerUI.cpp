#include "TransitionCustomizerUI.hpp"

void TransCustomizerModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    label = CCLabelBMFont::create(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str(), "bigFont.fnt");
    label->limitLabelWidth(160, 0.8f, 0);
    menu->addChildAtPosition(label, Anchor::Center, ccp(0, 8));

    left = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onLeft));
    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    menu->addChildAtPosition(left, Anchor::Center, ccp(-110, 8));

    right = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onRight));
    as<CCSprite*>(right->getNormalImage())->setFlipX(true);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
    menu->addChildAtPosition(right, Anchor::Center, ccp(110, 8));
}

void TransCustomizerModule::onLeft(CCObject*)
{
    Mod::get()->setSavedValue<int>("transition", Mod::get()->getSavedValue<int>("transition", 0) - 1);
    label->setString(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str());
    label->limitLabelWidth(160, 0.8f, 0);

    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
}

void TransCustomizerModule::onRight(CCObject*)
{
    Mod::get()->setSavedValue<int>("transition", Mod::get()->getSavedValue<int>("transition", 0) + 1);
    label->setString(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str());
    label->limitLabelWidth(160, 0.8f, 0);

    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
}

TransCustomizerModule::TransCustomizerModule()
{

}

void TransCustomizerModule::save() { }

void TransCustomizerModule::load() { }