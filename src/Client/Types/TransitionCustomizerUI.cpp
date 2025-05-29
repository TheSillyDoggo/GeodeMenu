#include "TransitionCustomizerUI.hpp"

void TransCustomizerModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    label = CCLabelBMFont::create(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str(), "bigFont.fnt");
    label->limitLabelWidth(160, 0.8f, 0);
    menu->addChildAtPosition(label, Anchor::Center, ccp(0, 8 + 20));

    left = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onLeft));
    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    menu->addChildAtPosition(left, Anchor::Center, ccp(-110, 8 + 20));

    right = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onRight));
    as<CCSprite*>(right->getNormalImage())->setFlipX(true);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
    menu->addChildAtPosition(right, Anchor::Center, ccp(110, 8 + 20));

    auto label2 = CCLabelBMFont::create("Transition Time:", "bigFont.fnt");
    label2->limitLabelWidth(200 - 40, 1.0f, 0);
    label2->setAnchorPoint(ccp(0, 0.5f));

    auto input = TextInput::create(90, "Time");
    input->setString(fmt::format("{:.02}", Mod::get()->getSavedValue<float>("transition-time", 0.5f)));
    input->setCommonFilter(CommonFilter::Float);

    input->setCallback([this](std::string const& str)
    {
        Mod::get()->setSavedValue<float>("transition-time", numFromString<float>(str).unwrapOr(Mod::get()->getSavedValue<float>("transition-time", 0.5f)));
    });

    menu->addChildAtPosition(input, Anchor::Right, ccp(-70, 8 - 35));
    menu->addChildAtPosition(label2, Anchor::Left, ccp(15 + 40, 8 - 35 + 1.5f));
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
    addToCache();
}

void TransCustomizerModule::save() { }

void TransCustomizerModule::load() { }