#include "SpeedhackNode.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "../../Utils/Num.hpp"
#include "../EditKeyConfigUI.hpp"
#include "../BetterAlertLayer.hpp"
#include "../../Localisation/LocalisationManager.hpp"

bool SpeedhackNode::init()
{
    if (!CategoryNode::init())
        return false;

    instance = this;

    auto menu = CCMenu::create();
    menu->setPosition(ccp(0, 0));

    input = TextInput::create(100, "Value");
    input->setString(Speedhack::get()->getText());
    input->setDelegate(this);
    input->setFilter("1234567890.");
    input->setPosition(getContentSize() / 2 + ccp(38, 75));

    input->getInputNode()->setUserObject("nwo5.scroll_inputs/input-type-float", CCNode::create());
    input->getInputNode()->setUserObject("nwo5.scroll_inputs/float-rounding", CCInteger::create(2));

    input->getInputNode()->setUserObject("nwo5.scroll_inputs/step-float", CCFloat::create(0.25f));
    input->getInputNode()->setUserObject("nwo5.scroll_inputs/small-step-float", CCFloat::create(0.1f));
    input->getInputNode()->setUserObject("nwo5.scroll_inputs/big-step-float", CCFloat::create(1.0f));

    auto speedLbl = AdvLabelBMFont::createWithLocalisation("speedhack/speed-label", "bigFont.fnt");
    speedLbl->setAnchorPoint(ccp(1, 0.5f));
    speedLbl->setScale(0.6f);
    speedLbl->setPosition(input->getPosition() + ccp(-70, 0));

    slider = BetterSlider::create(this, menu_selector(SpeedhackNode::onSliderMoved));
    slider->setPosition(input->getPosition() + ccp(-38, -35));
    slider->setScale(0.9f);
    slider->setRange(0.1f, 3.0f);
    slider->setSnapValuesRanged({ 1.0f });
    slider->setValueRanged(Speedhack::get()->getValue());

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

    presetKeybindMenu = CCMenu::create();
    presetKeybindMenu->setScale(0.41f);
    presetKeybindMenu->setAnchorPoint(ccp(0, 0));
    presetKeybindMenu->setPosition(5, 25);
    presetKeybindMenu->setContentWidth(700);

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

    deletePresetHelp = AdvLabelBMFont::createWithLocalisation("speedhack/delete-hint", "bigFont.fnt");
    deletePresetHelp->setScale(0.3f);

    deletePresetBG->setContentSize((deletePresetHelp->getScaledContentSize() + ccp(8, 8)) / 0.5f);
    deletePresetBG->setScale(0.5f);

    auto enabledNode = SpeedhackEnabled::get()->getNode();
    enabledNode->setPosition(slider->getPosition() + ccp(-8, -35));

    auto musicNode = SpeedhackMusic::get()->getNode();
    musicNode->setPosition(slider->getPosition() + ccp(-8, -35 - 30));

    auto gameplayNode = SpeedhackGameplay::get()->getNode();
    gameplayNode->setPosition(slider->getPosition() + ccp(-8, -35 - 30 - 30));

    updatePresets();

    menu->addChild(input);
    menu->addChild(speedLbl);
    menu->addChild(enabledNode);
    menu->addChild(musicNode);
    menu->addChild(gameplayNode);
    menu->addChild(trashBtn);
    menu->addChild(slider);
    this->addChild(menu);
    this->addChild(presetMenu);
    this->addChild(presetKeybindMenu);
    this->addChildAtPosition(presetConfigMenu, Anchor::BottomRight, ccp(0, 13));
    this->addChildAtPosition(deletePresetBG, Anchor::Bottom, ccp(0, 35));
    this->addChildAtPosition(deletePresetHelp, Anchor::Bottom, ccp(0, 35));
    return true;
}

void SpeedhackNode::updatePresets()
{
    auto presets = Speedhack::get()->getPresets();

    for (auto btn : presetBtns)
    {
        btn.first->removeFromParent();
    }

    presetBtns.clear();

    for (auto preset : presets)
    {
        auto spr = ButtonSprite::create(floatToStringMin2DP(preset.value).c_str(), "bigFont.fnt", presetDeleteMode ? "GJ_button_06.png" : "GJ_button_05.png");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, presetDeleteMode ? menu_selector(SpeedhackNode::onRemovePreset) : menu_selector(SpeedhackNode::onApplyPreset));
        presetMenu->addChild(btn);
        presetBtns.emplace(btn, preset.value);
    }

    presetMenu->updateLayout();
    deletePresetHelp->setVisible(presetDeleteMode);
    deletePresetBG->setVisible(presetDeleteMode);
    presetKeybindMenu->setVisible(!presetDeleteMode);

    sprMode->updateBGImage(presetDeleteMode ? "geode.loader/GE_button_03.png" : "GJ_button_06.png");
    sprMode->m_label->setString(presetDeleteMode ? "-" : "X");

    presetKeybindMenu->removeAllChildren();

    for (size_t i = 0; i < presetBtns.size(); i++)
    {
        auto oldBtn = presetMenu->getChildByIndex<CCMenuItemSpriteExtra*>(i);

        auto spr = CCSprite::create("keybinds.png"_spr);
        spr->setScale(2.2f);
        spr->setOpacity(presets[i].keyConfig.isValid() ? 255 : 150);

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SpeedhackNode::onSetPresetKeybind));
        btn->setTag(i);
        btn->setPosition(oldBtn->getPosition());

        presetKeybindMenu->addChild(btn);
    }
}

void SpeedhackNode::onSetPresetKeybind(CCObject* sender)
{
    auto presets = Speedhack::get()->getPresets();

    auto ui = EditKeyConfigUI::create([this, sender](KeyConfigStruct config)
    {
        auto pre = Speedhack::get()->getPresets();
        pre[sender->getTag()].keyConfig = config;
        Speedhack::get()->setPresets(pre);

        updatePresets();
    });

    ui->setDefaultConfig({ {}, Keycode::KEY_Unknown });
    ui->setStartConfig(presets[sender->getTag()].keyConfig);
    ui->show();
}

void SpeedhackNode::updateUI()
{
    input->setString(Speedhack::get()->getText());
    slider->setValueRanged(Speedhack::get()->getValue());

    updatePresets();
    ModuleNode::updateAllNodes(nullptr);
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
        auto lm = LocalisationManager::get();

        auto text = utils::string::replace(lm->getLocalisedString("speedhack/add-preset-exist/text"), "%s", floatToStringMin2DP(sh->getValue()));

        BetterAlertLayer::create(lm->getLocalisedString("speedhack/add-preset-exist/title").c_str(), text, lm->getLocalisedString("ui/ok-button").c_str())->show();

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

SpeedhackNode* SpeedhackNode::get()
{
    return instance;
}

SpeedhackNode::~SpeedhackNode()
{
    instance = nullptr;
}