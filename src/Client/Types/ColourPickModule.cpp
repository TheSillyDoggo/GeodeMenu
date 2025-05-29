#include "ColourPickModule.hpp"
#include "../../UI/TransLabelBMFont.hpp"

void ColourModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = TransLabelBMFont::create(name, "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(-15, 0));
    label->limitLabelWidth(110, 0.575f, 0.1f);

    btnSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    btnSpr->setColor(colour);
    btnSpr->setScale(0.75f);
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(ColourModule::onPickColourAndroid));

    btn->setPosition(pos + ccp(135, 0) + ccp(-15, 0));

    menu->addChild(btn);    
    menu->addChild(label);
}

void ColourModule::onPickColourAndroid(CCObject* sender)
{
    auto men = geode::ColorPickPopup::create(colour);

    men->setDelegate(this);
    men->show();
}

void ColourModule::updateColor(cocos2d::ccColor4B const& color)
{
    colour = ccc3(color.r, color.g, color.b);
    btnSpr->setColor(colour);
    save();
}

ColourModule::ColourModule(std::string name, std::string id, ccColor3B def)
{
    this->name = name;
    this->id = id;
    colour = def;
    this->def = def;

    this->load();

    addToCache();
}

void ColourModule::save()
{
    Mod::get()->setSavedValue<ccColor3B>(id + "_value", colour);
}

void ColourModule::load()
{
    colour = Mod::get()->getSavedValue<ccColor3B>(id + "_value", colour);
    save();
}