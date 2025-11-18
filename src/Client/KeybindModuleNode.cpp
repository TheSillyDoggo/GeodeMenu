#include "KeybindModuleNode.hpp"

KeybindModuleNode* KeybindModuleNode::create(KeybindModule* module)
{
    auto pRet = new KeybindModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void KeybindModuleNode::setup()
{
    auto mod = static_cast<KeybindModule*>(module);

    auto label = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(75, 0.575f, 0);

    keybindBG = CCScale9Sprite::create("geode.loader/black-square.png");
    keybindBG->setScale(0.5f);
    keybindLabel = CCLabelBMFont::create("", "goldFont.fnt");

    keybindBG->addChildAtPosition(keybindLabel, Anchor::Center);

    auto bg = CCMenuItemSpriteExtra::create(keybindBG, this, menu_selector(KeybindModuleNode::onChangeBind));

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(bg, Anchor::Right, ccp(-20, 0));

    updateNode();
}

void KeybindModuleNode::onChangeBind(CCObject* sender)
{
    FLAlertLayer::create("Change keybind", "Coming soon!", "OK")->show();
}

void KeybindModuleNode::updateNode()
{
    auto mod = static_cast<KeybindModule*>(module);
    auto str = CCKeyboardDispatcher::get()->keyToString(static_cast<enumKeyCodes>(mod->getKeyCode()));

    keybindLabel->setString(str ? str : "None");
}