#include "ColourPickModule.hpp"

void ColourModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(-15, 0));
    label->limitLabelWidth(110, 0.575f, 0.1f);

    btnSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    btnSpr->setColor(colour);
    btnSpr->setScale(0.75f);
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, menu, menu_selector(ColourModule::onPickColourAndroid));

    btn->setUserData(this);
    btn->setPosition(pos + ccp(135, 0) + ccp(-15, 0));

    menu->addChild(btn);    
    menu->addChild(label);
}

void ColourModule::onPickColourAndroid(CCObject* sender)
{
    auto v = as<ColourModule*>(as<CCNode*>(sender)->getUserData());

    if (!v->delegate)
    {
        v->delegate = new ColourModuleDelegate();
        v->delegate->mod = v;
    }

    auto men = geode::ColorPickPopup::create(v->colour);

    men->setDelegate(v->delegate);
    men->show();
}

void ColourModuleDelegate::updateColor(cocos2d::ccColor4B const& color)
{
    as<ColourModule*>(mod)->colour = ccc3(color.r, color.g, color.b);
    as<ColourModule*>(mod)->btnSpr->setColor(as<ColourModule*>(mod)->colour);
    as<ColourModule*>(mod)->save();
}

ColourModule::ColourModule(std::string name, std::string id, ccColor3B def)
{
    this->name = name;
    this->id = id;
    colour = def;
    this->def = def;

    this->load();
}

void ColourModule::save()
{
    Mod::get()->setSavedValue<ccColor3B>(id + "_value", colour);
}

void ColourModule::load()
{
    Mod::get()->getSavedValue<ccColor3B>(id + "_value", colour);
}