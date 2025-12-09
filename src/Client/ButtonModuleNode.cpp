#include "ButtonModuleNode.hpp"

ButtonModuleNode* ButtonModuleNode::create(ButtonModule* module)
{
    auto pRet = new ButtonModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ButtonModuleNode::onButtonClicked(CCObject* sender)
{
    static_cast<ButtonModule*>(module)->onClick();
}

void ButtonModuleNode::setup()
{
    auto lbl = CCLabelBMFont::create(module->getName().c_str(), "bigFont.fnt");
    lbl->limitLabelWidth(180, 0.8f, 0);

    auto spr = CCScale9Sprite::create("GJ_button_05.png");
    spr->setContentSize(ccp(190, 26 / 0.7f));
    spr->setScale(0.7f);

    spr->addChildAtPosition(lbl, Anchor::Center);

    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ButtonModuleNode::onButtonClicked));
    btn->m_scaleMultiplier = 1.1f;

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.55f + 0.35f);

    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModuleNode::onInfo));

    infoSpr->setScale(infoSpr->getScale() - 0.35f);

    this->addChildAtPosition(btn, Anchor::Center, ccp(-4, 0));
    this->addChildAtPosition(infoBtn, Anchor::Center, ccp(spr->getScaledContentWidth() / 2 + 4, 9));
}