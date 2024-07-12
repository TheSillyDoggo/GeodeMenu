#include "SliderModule.hpp"

void SliderModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(-15, 0));
    label->limitLabelWidth(80, 0.575f, 0.1f);

    slider = Slider::create(menu, menu_selector(SliderModule::onSliderChanged));
    slider->setValue(value);
    slider->updateBar();
    slider->getThumb()->setUserData(this);

    slider->setAnchorPoint(ccp(0, 0));
    slider->setScale(0.5f);
    slider->setScaleX(0.35f);
    slider->setPosition(pos + ccp(135, 0) + ccp(-15 - 14, 0));
    
    as<CCSprite*>(slider->getThumb()->getChildren()->objectAtIndex(0))->setScaleX(0.5f / 0.35f);
    as<CCSprite*>(slider->getThumb()->getChildren()->objectAtIndex(1))->setScaleX(0.5f / 0.35f);
    
    menu->addChild(slider);
    menu->addChild(label);
}

void SliderModule::onSliderChanged(CCObject* sender)
{
    auto x = as<SliderModule*>(as<SliderThumb*>(sender)->getUserData());

    x->value = as<SliderThumb*>(sender)->getValue();
    
    x->save();
}

SliderModule::SliderModule(std::string name, std::string id, float def)
{
    this->name = name;
    this->id = id;
    value = def;

    this->load();
}

void SliderModule::save()
{
    Mod::get()->setSavedValue<float>(id + "_value", value);
}

void SliderModule::load()
{
    value = Mod::get()->getSavedValue<float>(id + "_value", value);
}