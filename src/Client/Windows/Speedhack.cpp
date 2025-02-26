#include "Speedhack.hpp"

using namespace geode::prelude;

void Speedhack::onClear(CCObject* sender)
{
    SpeedhackTop::instance->text = "";

    input->setString("");
    slider->setValue(unscaleValue(1));
}

float Speedhack::scaleValue(float originalValue)
{
    float minValue = 0.1;
    float maxValue = 3.0;
    float scaledValue = (maxValue - minValue) * originalValue + minValue;
    return scaledValue;
}

float Speedhack::unscaleValue(float scaledValue)
{
    float minValue = 0.1;
    float maxValue = 3.0;
    float originalValue = (scaledValue - minValue) / (maxValue - minValue);
    return originalValue;
}

void Speedhack::sliderChanged(CCObject* sender)
{
    float v = ((slider->getThumb()->getPositionX() + 100) / 200.0f);

    auto str = utils::numToString<float>(scaleValue(v), 2);

    input->setString(str);
    SpeedhackTop::instance->text = str;

    SpeedhackTop::instance->save();
    SpeedhackTop::instance->onChange();
}

void Speedhack::onPreset(CCObject* sender)
{
    float value = numFromString<float>(as<CCNode*>(sender)->getID(), 2).unwrap();

    if (isDeleting)
    {
        presets.erase(std::find(presets.begin(), presets.end(), value));

        updatePresets();
        updatePresetsSprites(6);

        Mod::get()->setSavedValue<std::vector<float>>("speedhack-presets", presets);

        return;
    }

    SpeedhackTop::instance->text = as<CCNode*>(sender)->getID();
    input->setString(as<CCNode*>(sender)->getID());
    slider->setValue(unscaleValue(value));
    
    SpeedhackTop::instance->save();
    SpeedhackTop::instance->onChange();
}

void Speedhack::cocosCreate(CCMenu* menu)
{
    isDeleting = false;
    presets = Mod::get()->getSavedValue<std::vector<float>>("speedhack-presets", { 0.1f, 0.25f, 0.3f, 0.50f, 0.75f, 1, 1.50f, 2 });

    float v = 1.0f;

    auto x = numFromString<float>(SpeedhackTop::instance->text);

    if (x.isOk())
    {
        v = x.unwrap();
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

    slider = Slider::create(this, menu_selector(Speedhack::sliderChanged));
    slider->setPosition(ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 80));
    slider->setScale(0.875f);
    slider->setContentSize(ccp(0, 0));
    slider->ignoreAnchorPointForPosition(false);
    slider->setValue(clampf(unscaleValue(v), 0, 1));
    menu->addChild(slider, 2);

    modules[0]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 50) - ccp(180 / 2, 0) + ccp(10, 0));

    input = menu->getChildByType<TextInput>(0);

    for (size_t i = 1; i < modules.size(); i++)
    {
        modules[i]->makeAndroid(menu, (ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110 - (30 * i)) - ccp(180 / 2, 0) + ccp(20, 0)) + ccp(0, 27.5f));
    }

    auto trash = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), this, menu_selector(Speedhack::onClear));
    trash->m_baseScale = 0.725f;
    trash->setScale(0.725f);
    trash->setPosition(ccp((menu->getContentSize().width / 2) + (180 / 2) + 20, menu->getContentSize().height - 50));
    menu->addChild(trash);

    presetMenu = CCMenu::create();
    presetMenu->setScale(0.41f);
    presetMenu->setAnchorPoint(ccp(0, 0.5f));
    presetMenu->setPosition(5, 13);
    presetMenu->setContentWidth(700);
    presetMenu->setLayout(RowLayout::create()->setGap(15)->setAutoScale(true));

    updatePresets();

    auto configMenu = CCMenu::create();
    configMenu->setPosition(menu->getContentWidth(), 13);
    configMenu->setScale(0.41f);
    configMenu->setContentSize(CCPointZero);

    auto sprP = ButtonSprite::create("+", "bigFont.fnt", "GJ_button_05.png");
    auto btnP = CCMenuItemSpriteExtra::create(sprP, this, menu_selector(Speedhack::onNewPreset));
    btnP->setPositionX(-30);

    sprD = ButtonSprite::create("X", "bigFont.fnt", "GJ_button_06.png");
    auto btnD = CCMenuItemSpriteExtra::create(sprD, this, menu_selector(Speedhack::onDeletePreset));
    btnD->setPositionX(-80);

    configMenu->addChild(btnP);
    configMenu->addChild(btnD);

    menu->addChild(presetMenu);
    menu->addChild(configMenu);
}

void Speedhack::updatePresets()
{
    std::sort(presets.begin(), presets.end(), [](float a, float b)
    {
        return a < b;
    });

    presetMenu->removeAllChildren();
    presetBtns.clear();

    for (auto preset : presets)
    {
        auto spr = ButtonSprite::create(utils::numToString(preset, 2).c_str(), "bigFont.fnt", "GJ_button_05.png");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(Speedhack::onPreset));
        btn->setID(numToString(preset, 2));

        presetMenu->addChild(btn);
        presetBtns.push_back(btn);
    }

    presetMenu->updateLayout();
}

void Speedhack::updatePresetsSprites(int spr)
{
    for (auto btn : presetBtns)
    {
        as<ButtonSprite*>(btn->getNormalImage())->updateBGImage(fmt::format("GJ_button_0{}.png", spr).c_str());
    }
}

void Speedhack::onNewPreset(CCObject* sender)
{
    float v = numFromString<float>(input->getString()).unwrapOr(1.0f);

    if (std::find(presets.begin(), presets.end(), v) != presets.end())
    {
        FLAlertLayer::create("Error", fmt::format("There is already a preset with the value <cc>{:.2f}</c>.", v), "OK")->show();

        return;
    }

    presets.push_back(v);

    updatePresets();
    updatePresetsSprites(isDeleting ? 6 : 5);

    Mod::get()->setSavedValue<std::vector<float>>("speedhack-presets", presets);
}

void Speedhack::onDeletePreset(CCObject* sender)
{
    isDeleting = !isDeleting;
    updatePresetsSprites(isDeleting ? 6 : 5);
    sprD->updateBGImage(isDeleting ? "geode.loader/GE_button_03.png" : "GJ_button_06.png");
    sprD->m_label->setString(isDeleting ? "-" : "X");
}

void Speedhack::textChanged(CCTextInputNode* p0)
{
    SpeedhackTop::instance->text = p0->getString();

    float v = 1.0f;

    v = numFromString<float>(p0->getString()).unwrapOr(v);

    if (v < 0.01f)
        v = 0.01f;

    if (v > 99999)
        v = 99999;

    slider->setValue(clampf(unscaleValue(v), 0, 1));

    modules[0]->save();
}