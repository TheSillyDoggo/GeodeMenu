#include "EnumModuleNode.hpp"
#include "EnumModule.hpp"
#include <LocalisationManager.hpp>

using namespace qolmod;

EnumModuleNode* EnumModuleNode::create(EnumModule* module)
{
    auto pRet = new EnumModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void EnumModuleNode::setup()
{
    label = AdvLabelBMFont::createWithString("", "bigFont.fnt");

    leftBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(EnumModuleNode::onLeft));
    rightBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(EnumModuleNode::onRight));

    leftBtn->setRepeatEnabled(true);
    rightBtn->setRepeatEnabled(true);

    leftBtn->getNormalImage()->setScale(0.6f);
    rightBtn->getNormalImage()->setScaleX(-0.6f);
    rightBtn->getNormalImage()->setScaleY(0.6f);

    this->addChildAtPosition(label, Anchor::Center);
    this->addChildAtPosition(leftBtn, Anchor::Left, ccp(13.5f, 0));
    this->addChildAtPosition(rightBtn, Anchor::Right, ccp(-13.5f, 0));
    updateNode();
}

void EnumModuleNode::updateNode()
{
    auto mod = static_cast<EnumModule*>(module);
    auto names = mod->getDisplayNames();

    label->setString(LocalisationManager::get()->getLocalisedString(
        fmt::format("enums/{}", names[mod->getValue()])
    ).c_str());
    label->limitLabelWidth(100, 0.5f, 0);

    updateButtonEnabled(leftBtn, (mod->getValue() != names.begin()->first));
    updateButtonEnabled(rightBtn, (mod->getValue() != std::prev/*ter*/(names.end())->first));
}

void EnumModuleNode::updateButtonEnabled(Button* btn, bool enabled)
{
    btn->setEnabled(enabled);
    btn->setColor(enabled ? ccWHITE : ccc3(150, 150, 150));
}

void EnumModuleNode::onLeft(CCObject* sender)
{
    auto mod = static_cast<EnumModule*>(module);
    
    mod->setPrev();
    updateNode();
}

void EnumModuleNode::onRight(CCObject* sender)
{
    auto mod = static_cast<EnumModule*>(module);

    mod->setNext();
    updateNode();
}