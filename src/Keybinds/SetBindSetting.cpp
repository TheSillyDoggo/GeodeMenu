#include "SetBindSetting.hpp"
#include "KeyInfoPopup.hpp"
#include "RecordKeyPopup.hpp"

bool SetBindNode::init(SetBindValue* value, float width)
{
    if (!SettingNode::init(value))
        return false;

    this->setContentHeight(35);
    this->setContentWidth(width);

    startButtons = value->buttons;
    buttons = value->buttons;

    auto label = CCLabelBMFont::create("Open Menu", "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.5f);

    menu = CCMenu::create();
    menu->setContentSize(ccp(6969, 0));
    menu->setAnchorPoint(ccp(1, 0.5f));
    menu->setScale(0.75f);

    auto set = ButtonSprite::create("Add", "bigFont.fnt", "GJ_button_04.png");
    set->setScale(0.65f);

    auto setBtn = CCMenuItemSpriteExtra::create(set, this, menu_selector(SetBindNode::onAddBtn));
    menu->addChild(setBtn);

    for (auto btn : value->buttons)
    {
        auto lbl = CCLabelBMFont::create(nameForKey(btn).c_str(), "bigFont.fnt");

        auto lblBG = CCScale9Sprite::create("geode.loader/white-square.png");
        lblBG->setColor(ccc3(0, 0, 0));
        lblBG->setContentWidth(lbl->getScaledContentSize().width + 5);
        lblBG->addChildAtPosition(lbl, Anchor::Center);
        lblBG->setScale(0.65f);

        auto lblBtn = CCMenuItemSpriteExtra::create(lblBG, this, menu_selector(SetBindNode::onSetBtn));
        lblBtn->setTag(btn);

        menu->addChild(lblBtn);

        btns.insert(std::pair<int, CCNode*>(btn, lblBtn));
    }

    menu->setLayout(AxisLayout::create()->setAxis(Axis::Row)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End));

    this->addChildAtPosition(label, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(menu, Anchor::Right, ccp(-10, 0));
    return true;
}

void SetBindNode::onAddBtn(CCObject* sender)
{
    auto addPopup = RecordKeyPopup::create(menu_selector(SetBindNode::onKeySubmit));
    addPopup->setUserData(this);

    CCScene::get()->addChild(addPopup, CCScene::get()->getHighestChildZ() + 1);
}

void SetBindNode::onSetBtn(CCObject* sender)
{
    auto keyInfo = KeyInfoPopup::createWithKeyAndBind(as<CCNode*>(sender)->getTag(), this);
    CCScene::get()->addChild(keyInfo, CCScene::get()->getHighestChildZ() + 1);
}

void SetBindNode::onKeySubmit(CCObject* sender)
{
    auto key = as<CCNode*>(sender)->getTag();
    auto buttons = as<SetBindNode*>(as<CCNode*>(sender)->getUserData())->buttons;
    auto ins = as<SetBindNode*>(as<CCNode*>(sender)->getUserData());

    if (key == -1)
        return FLAlertLayer::create("Keybind Error", "<cr>Unsupported key</c>\nCocos2d-x doesn't support this key.", "OK")->show();

    if (std::find(buttons.begin(), buttons.end(), key) != buttons.end())
        return FLAlertLayer::create("Keybind Error", "You already have this key added", "OK")->show();

    log::info("adding key {}", key);

    ins->buttons.push_back(key);
    ins->dispatchChanged();

    auto lbl = CCLabelBMFont::create(ins->nameForKey(key).c_str(), "bigFont.fnt");

    auto lblBG = CCScale9Sprite::create("geode.loader/black-square.png");
    lblBG->setContentWidth(lbl->getScaledContentSize().width + 5);
    lblBG->addChildAtPosition(lbl, Anchor::Center);
    lblBG->setScale(0.65f);

    auto lblBtn = CCMenuItemSpriteExtra::create(lblBG, this, menu_selector(SetBindNode::onSetBtn));
    lblBtn->setTag(key);

    ins->menu->addChild(lblBtn);
    ins->menu->updateLayout();

    ins->btns.insert(std::pair<int, CCNode*>(key, lblBtn));
}

std::string SetBindNode::nameForKey(int key)
{
    if (key == -1)
        return "Unknown";

    auto k = CCKeyboardDispatcher::get()->keyToString(as<enumKeyCodes>(key));

    if (k == nullptr)
        return "Unknown";

    return std::string(k);
}

SettingNode* SetBindValue::createNode(float width)
{
    return SetBindNode::create(this, width);
}

$execute
{
    Mod::get()->registerCustomSetting(
        "set-bind",
        std::make_unique<SetBindValue>("set-bind", Mod::get()->getID())
    );
}