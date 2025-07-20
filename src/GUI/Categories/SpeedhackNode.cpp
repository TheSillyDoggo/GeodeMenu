#include "SpeedhackNode.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "../../Utils/Num.hpp"

bool SpeedhackNode::init()
{
    if (!CategoryNode::init())
        return false;

    auto menu = CCMenu::create();
    menu->setPosition(ccp(0, 0));

    input = TextInput::create(100, "Speed");
    input->setString(Speedhack::get()->getText());
    input->setDelegate(this);
    input->setFilter("1234567890.");
    input->setPosition(getContentSize() / 2 + ccp(38, 75));

    auto speedLbl = CCLabelBMFont::create("Speed:", "bigFont.fnt");
    speedLbl->setAnchorPoint(ccp(1, 0.5f));
    speedLbl->setScale(0.6f);
    speedLbl->setPosition(input->getPosition() + ccp(-70, 0));

    slider = Slider::create(this, menu_selector(SpeedhackNode::onSliderMoved));
    slider->setPosition(input->getPosition() + ccp(-38, -35));
    slider->setScale(0.9f);
    slider->setAnchorPoint(ccp(0, 0));
    slider->setValue(utils::clamp<float>(unscaleFloat(Speedhack::get()->getRealValue(), 0.1f, 3.0f), 0, 1));

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
    return true;
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
    Speedhack::get()->setText(fmt::format("{:.02f}", scaleFloat(slider->getValue(), 0.1f, 3.0f)));
    input->setString(Speedhack::get()->getText());
}

void SpeedhackNode::onTrash(CCObject* sender)
{
    Speedhack::get()->setText("");
    slider->setValue(utils::clamp<float>(unscaleFloat(Speedhack::get()->getRealValue(), 0.1f, 3.0f), 0, 1));
    input->setString("");
}

void SpeedhackNode::textChanged(CCTextInputNode* node)
{
    Speedhack::get()->setText(input->getString());
    slider->setValue(utils::clamp<float>(unscaleFloat(Speedhack::get()->getRealValue(), 0.1f, 3.0f), 0, 1));
}