#include "SpeedhackNode.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "../../Utils/Num.hpp"

bool SpeedhackNode::init()
{
    if (!CategoryNode::init())
        return false;

    auto menu = CCMenu::create();
    menu->setPosition(ccp(0, 0));

    input = TextInput::create(100, "Value");
    input->setString(Speedhack::get()->getText());
    input->setDelegate(this);
    input->setFilter("1234567890.");
    input->setPosition(getContentSize() / 2 + ccp(38, 75));

    auto speedLbl = CCLabelBMFont::create("Speed:", "bigFont.fnt");
    speedLbl->setAnchorPoint(ccp(1, 0.5f));
    speedLbl->setScale(0.6f);
    speedLbl->setPosition(input->getPosition() + ccp(-70, 0));

    slider = BetterSlider::create(this, menu_selector(SpeedhackNode::onSliderMoved));
    slider->setPosition(input->getPosition() + ccp(-38, -35));
    slider->setScale(0.9f);
    slider->setRange(0.1f, 3.0f);
    slider->setSnapValuesRanged({ 1.0f });
    slider->setValueRanged(Speedhack::get()->getValue());

    enabledBtn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SpeedhackNode::onToggleEnabled), 0.75f);
    enabledBtn->toggle(Speedhack::get()->getEnabled());
    enabledBtn->setPosition(slider->getPosition() + ccp(-80, -35));

    auto enabledLbl = CCLabelBMFont::create("Enabled", "bigFont.fnt");
    enabledLbl->setAnchorPoint(ccp(0, 0.5f));
    enabledLbl->setScale(0.5f);
    enabledLbl->setPosition(enabledBtn->getPosition() + ccp(18, 0));

    musicBtn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SpeedhackNode::onToggleMusic), 0.75f);
    musicBtn->toggle(Speedhack::get()->getMusicEnabled());
    musicBtn->setPosition(slider->getPosition() + ccp(-80, -35 - 30));

    auto musicLbl = CCLabelBMFont::create("Speedhack Music", "bigFont.fnt");
    musicLbl->setAnchorPoint(ccp(0, 0.5f));
    musicLbl->setScale(0.5f);
    musicLbl->setPosition(musicBtn->getPosition() + ccp(18, 0));

    gameplayBtn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SpeedhackNode::onToggleGameplay), 0.75f);
    gameplayBtn->toggle(Speedhack::get()->getGameplayEnabled());
    gameplayBtn->setPosition(slider->getPosition() + ccp(-80, -35 - 30 - 30));

    auto gameplayLbl = CCLabelBMFont::create("Gameplay only", "bigFont.fnt");
    gameplayLbl->setAnchorPoint(ccp(0, 0.5f));
    gameplayLbl->setScale(0.5f);
    gameplayLbl->setPosition(gameplayBtn->getPosition() + ccp(18, 0));

    auto trashSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    trashSpr->setScale(0.75f);

    trashBtn = CCMenuItemSpriteExtra::create(trashSpr, this, menu_selector(SpeedhackNode::onTrash));
    trashBtn->setPosition(input->getPosition() + ccp(75, 0));

    presetMenu = CCMenu::create();
    presetMenu->setScale(0.41f);
    presetMenu->setAnchorPoint(ccp(0, 0.5f));
    presetMenu->setPosition(5, 13);
    presetMenu->setContentWidth(700);
    presetMenu->setLayout(RowLayout::create()->setGap(15)->setAutoScale(true));

    auto presetConfigMenu = CCMenu::create();
    presetConfigMenu->setScale(0.41f);
    presetConfigMenu->setAnchorPoint(ccp(0, 0));

    auto btnNew = CCMenuItemSpriteExtra::create(ButtonSprite::create("+", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(SpeedhackNode::onAddNewPreset));
    btnNew->setPositionX(-30);

    sprMode = ButtonSprite::create("X", "bigFont.fnt", "GJ_button_06.png");
    auto btnMode = CCMenuItemSpriteExtra::create(sprMode, this, menu_selector(SpeedhackNode::onChangePresetMode));
    btnMode->setPositionX(-80);

    presetConfigMenu->addChild(btnNew);
    presetConfigMenu->addChild(btnMode);

    deletePresetBG = CCScale9Sprite::create("square02b_small.png");
    deletePresetBG->setColor(ccc3(0, 0, 0));
    deletePresetBG->setOpacity(100);

    deletePresetHelp = CCLabelBMFont::create("Tap a preset to remove it", "bigFont.fnt");
    deletePresetHelp->setScale(0.3f);

    deletePresetBG->setContentSize((deletePresetHelp->getScaledContentSize() + ccp(8, 8)) / 0.5f);
    deletePresetBG->setScale(0.5f);

    updatePresets();

    menu->addChild(input);
    menu->addChild(speedLbl);
    menu->addChild(enabledBtn);
    menu->addChild(enabledLbl);
    menu->addChild(gameplayBtn);
    menu->addChild(gameplayLbl);
    menu->addChild(musicBtn);
    menu->addChild(musicLbl);
    menu->addChild(trashBtn);
    menu->addChild(slider);
    this->addChild(menu);
    this->addChild(presetMenu);
    this->addChildAtPosition(presetConfigMenu, Anchor::BottomRight, ccp(0, 13));
    this->addChildAtPosition(deletePresetBG, Anchor::Bottom, ccp(0, 35));
    this->addChildAtPosition(deletePresetHelp, Anchor::Bottom, ccp(0, 35));
    return true;
}

void SpeedhackNode::updatePresets()
{
    for (auto btn : presetBtns)
    {
        btn.first->removeFromParent();
    }

    presetBtns.clear();

    for (auto preset : Speedhack::get()->getPresets())
    {
        auto spr = ButtonSprite::create(floatToStringMin2DP(preset.value).c_str(), "bigFont.fnt", presetDeleteMode ? "GJ_button_06.png" : "GJ_button_05.png");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, presetDeleteMode ? menu_selector(SpeedhackNode::onRemovePreset) : menu_selector(SpeedhackNode::onApplyPreset));
        presetMenu->addChild(btn);
        presetBtns.emplace(btn, preset.value);
    }

    presetMenu->updateLayout();
    deletePresetHelp->setVisible(presetDeleteMode);
    deletePresetBG->setVisible(presetDeleteMode);

    sprMode->updateBGImage(presetDeleteMode ? "geode.loader/GE_button_03.png" : "GJ_button_06.png");
    sprMode->m_label->setString(presetDeleteMode ? "-" : "X");
}

void SpeedhackNode::onApplyPreset(CCObject* sender)
{
    float val = presetBtns[static_cast<CCMenuItemSpriteExtra*>(sender)];
    auto str = floatToStringMin2DP(val);

    Speedhack::get()->setText(str);
    slider->setValueRanged(val);
    input->setString(str);
}

void SpeedhackNode::onAddNewPreset(CCObject* sender)
{
    auto sh = Speedhack::get();

    if (sh->hasPreset(sh->getValue()))
    {
        FLAlertLayer::create("Speedhack Preset", fmt::format("There's already a preset with the value <cc>{}</c>", floatToStringMin2DP(sh->getValue())), "OK")->show();

        return;
    }

    auto presets = sh->getPresets();
    presets.push_back(SpeedhackPreset(sh->getValue()));
    sh->setPresets(presets);

    updatePresets();
}

void SpeedhackNode::onRemovePreset(CCObject* sender)
{
    auto sh = Speedhack::get();

    std::vector<SpeedhackPreset> presets = {};

    for (auto pre : sh->getPresets())
    {
        if (pre.value == presetBtns[static_cast<CCMenuItemSpriteExtra*>(sender)])
            continue;

        presets.push_back(pre);
    }

    sh->setPresets(presets);
    updatePresets();
}

void SpeedhackNode::onChangePresetMode(CCObject* sender)
{
    presetDeleteMode = !presetDeleteMode;

    updatePresets();
}

void SpeedhackNode::onToggleEnabled(CCObject* sender)
{
    Speedhack::get()->setEnabled(!Speedhack::get()->getEnabled());
}

void SpeedhackNode::onToggleMusic(CCObject* sender)
{
    Speedhack::get()->setMusicEnabled(!Speedhack::get()->getMusicEnabled());
}

void SpeedhackNode::onToggleGameplay(CCObject* sender)
{
    Speedhack::get()->setGameplayEnabled(!Speedhack::get()->getGameplayEnabled());
}

void SpeedhackNode::onSliderMoved(CCObject* sender)
{
    Speedhack::get()->setText(fmt::format("{:.02f}", slider->getValueRanged()));
    input->setString(Speedhack::get()->getText());
}

void SpeedhackNode::onTrash(CCObject* sender)
{
    Speedhack::get()->setText("");
    slider->setValueRanged(Speedhack::get()->getValue());
    input->setString("");
}

void SpeedhackNode::textChanged(CCTextInputNode* node)
{
    Speedhack::get()->setText(input->getString());
    slider->setValueRanged(Speedhack::get()->getValue());
}