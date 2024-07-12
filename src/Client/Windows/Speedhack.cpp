#include "Speedhack.h"

using namespace geode::prelude;

void Speedhack::clear(CCObject* sender) {
    SpeedhackTop::instance->text = "";
    auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent(), 0);
    inp->setString("");
    slider->setValue(unscaleValue(1));
}

float Speedhack::scaleValue(float originalValue) {
    float minValue = 0.1;
    float maxValue = 3.0;
    float scaledValue = (maxValue - minValue) * originalValue + minValue;
    return scaledValue;
}

float Speedhack::unscaleValue(float scaledValue) {
    float minValue = 0.1;
    float maxValue = 3.0;
    float originalValue = (scaledValue - minValue) / (maxValue - minValue);
    return originalValue;
}

void Speedhack::sliderChanged(CCObject* sender) {
    float v = ((slider->getThumb()->getPositionX() + 100) / 200.0f);

    std::stringstream ss;
    ss << round(scaleValue(v) * 100.0) / 100.0;

    auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent()->getParent()->getParent(), 0);
    inp->setString(ss.str().c_str());
    SpeedhackTop::instance->text = ss.str();

    SpeedhackTop::instance->save();
    SpeedhackTop::instance->onChange();
}

void Speedhack::onPreset(CCObject* sender) {
    float value = numFromString<float>(as<CCNode*>(sender)->getID(), 2).value();

    SpeedhackTop::instance->text = as<CCNode*>(sender)->getID();
    auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent()->getParent(), 0);
    inp->setString(as<CCNode*>(sender)->getID());
    slider->setValue(unscaleValue(value));
    
    SpeedhackTop::instance->save();
    SpeedhackTop::instance->onChange();
}

void Speedhack::cocosCreate(CCMenu* menu) {
    float v = 1.0f;

    auto x = numFromString<float>(SpeedhackTop::instance->text);

    if (x.isOk())
    {
        v = x.value();
    }

    if (v < 0.01f)
        v = 0.01f;

    if (v > 99999)
        v = 99999;            
    
    auto back = CCScale9Sprite::create("square02b_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setColor(ccc3(0, 0, 0));
    back->setOpacity(100);
    menu->addChild(back);

    slider = Slider::create(menu, menu_selector(Speedhack::sliderChanged));
    slider->setPosition(ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 80));
    slider->setScale(0.875f);
    slider->setContentSize(ccp(0, 0));
    slider->ignoreAnchorPointForPosition(false);
    slider->setValue(clampf(unscaleValue(v), 0, 1));
    menu->addChild(slider, 2);

    modules[0]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 50) - ccp(180 / 2, 0) + ccp(10, 0));

    for (size_t i = 1; i < modules.size(); i++)
    {
        modules[i]->makeAndroid(menu, (ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110 - (30 * i)) - ccp(180 / 2, 0) + ccp(20, 0)) + ccp(0, 27.5f));
    }

    //static_cast<geode::InputNode*>(menu->getChildByID("speedhack-top"))->getInput()->setDelegate(this);

    auto trash = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), menu, menu_selector(Speedhack::clear));
    trash->m_baseScale = 0.725f;
    trash->setScale(0.725f);
    trash->setPosition(ccp((menu->getContentSize().width / 2) + (180 / 2) + 20, menu->getContentSize().height - 50));
    menu->addChild(trash);

    auto presetMenu = CCMenu::create();
    presetMenu->setScale(0.41f);
    presetMenu->setAnchorPoint(ccp(0.5f, 0.5f));
    presetMenu->setPosition(menu->getContentWidth() / 2, 13);
    presetMenu->setContentWidth(6969);
    presetMenu->setLayout(RowLayout::create()->setGap(15)->setAutoScale(false));

    for (auto preset : presets)
    {
        auto btn = ButtonSprite::create(utils::numToString(preset, 2).c_str(), "bigFont.fnt", "GJ_button_05.png");
        auto act = CCMenuItemSpriteExtra::create(btn, menu, menu_selector(Speedhack::onPreset));
        act->setID(numToString(preset, 2));

        presetMenu->addChild(act);
    }           

    presetMenu->updateLayout();
    menu->addChild(presetMenu);
}

void Speedhack::textChanged(CCTextInputNode* p0) {
    SpeedhackTop::instance->text = p0->getString();

    float v = 1.0f;

    if (SpeedhackTop::instance->text.size() != 0 && !SpeedhackTop::instance->text.ends_with("."))
    {
        v = std::stof(SpeedhackTop::instance->text);
    }

    if (v < 0.01f)
        v = 0.01f;

    if (v > 99999)
        v = 99999;

    slider->setValue(clampf(unscaleValue(v), 0, 1));

    modules[0]->save();
}