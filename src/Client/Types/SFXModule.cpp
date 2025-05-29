#include "SFXModule.hpp"
#include "../../UI/TransLabelBMFont.hpp"
#include "../../Layers/SFXModuleManageLayer.hpp"
#include <random>

SFXModule::SFXModule(std::string name, std::string id)
{
    this->name = name;
    this->id = id;
    
    load();

    addToCache();
}

void SFXModule::save()
{
    Mod::get()->setSavedValue<std::vector<std::string>>(fmt::format("{}_values", id), sfxs);
}

void SFXModule::load()
{
    sfxs = Mod::get()->getSavedValue<std::vector<std::string>>(fmt::format("{}_values", id), sfxs);
}

void SFXModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = TransLabelBMFont::create(name, "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos - ccp(10, 0));
    label->limitLabelWidth(110, 0.575f, 0.1f);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
    spr->setScale(0.45f);

    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SFXModule::onPickSFX));
    btn->setPosition(pos + ccp(125, 0));

    menu->addChild(label);
    menu->addChild(btn);
}

void SFXModule::onPickSFX(CCObject* sender)
{
    SFXModuleManageLayer::addToScene(this);
}

void SFXModule::playRandomSFX()
{
    if (getSFXCount() == 0)
        return;

    FMODAudioEngine::get()->playEffect(getRandomSFX());
}

int SFXModule::getSFXCount()
{
    return sfxs.size();
}

std::string SFXModule::getRandomSFX()
{
    if (sfxs.size() == 0)
        return "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, sfxs.size() -1);

    return sfxs[distr(gen)];
}