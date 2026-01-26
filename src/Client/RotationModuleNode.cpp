#include "RotationModuleNode.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../GUI/SetupRotationUI.hpp"

RotationModuleNode* RotationModuleNode::create(RotationModule* module)
{
    auto pRet = new RotationModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void RotationModuleNode::setup()
{
    auto mod = static_cast<RotationModule*>(module);

    auto label = AdvLabelBMFont::createWithString(mod->getName(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(120, 0.5f, 0);

    auto bg = CCSprite::create("GJ_button_01.png");
    bg->setScale(23.725f / bg->getContentHeight());

    auto overlay = CCSprite::createWithSpriteFrameName("edit_enableRotateBtn_001.png");
    bg->addChildAtPosition(overlay, Anchor::Center);
    
    auto btn = CCMenuItemSpriteExtra::create(bg, this, menu_selector(RotationModuleNode::onChangeRotation));

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(btn, Anchor::Left, ccp(label->getScaledContentWidth() + 25, 0));
}

void RotationModuleNode::onChangeRotation(CCObject* sender)
{
    auto mod = static_cast<RotationModule*>(module);

    auto ui = SetupRotationUI::create([this, mod](float v)
    {
        mod->setRotation(v);
    });
    ui->setDefaultRot(mod->getDefaultValue());
    ui->setStartRot(mod->getRotationDegrees());
    ui->setRunCallbackOnEveryChange(true);
    ui->show();
}

void RotationModuleNode::updateNode()
{
    
}