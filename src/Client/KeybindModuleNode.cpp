#include "KeybindModuleNode.hpp"
#include "../GUI/KeycodeListenerLayer.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

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

    auto label = AdvLabelBMFont::createWithString(mod->getName(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(120, 0.5f, 0);

    keycodeNode = KeycodeNode::create(0);

    auto bg = CCMenuItemSpriteExtra::create(keycodeNode, this, menu_selector(KeybindModuleNode::onChangeBind));

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(bg, Anchor::Right, ccp(-20, 0));

    updateNode();
}

void KeybindModuleNode::onChangeBind(CCObject* sender)
{
    auto mod = static_cast<KeybindModule*>(module);

    auto layer = KeycodeListenerLayer::create({mod->getKeyCode(), mod->getDefaultKeyCode(), true, !mod->isBindRequired()}, [this, mod](int code)
    {
        mod->setKeyCode(code);

        updateAllNodes(nullptr);
    });
    layer->show();
}

void KeybindModuleNode::updateNode()
{
    auto mod = static_cast<KeybindModule*>(module);

    keycodeNode->setKeycode(mod->getKeyCode());
}