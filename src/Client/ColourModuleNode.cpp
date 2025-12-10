#include "ColourModuleNode.hpp"
#include "../GUI/SetupColourConfigUI.hpp"

ColourModuleNode* ColourModuleNode::create(ColourModule* module)
{
    auto pRet = new ColourModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
        
void ColourModuleNode::update(float dt)
{
    auto mod = static_cast<ColourModule*>(module);

    colourSpr->setColor(mod->getColour());
}

void ColourModuleNode::setup()
{
    auto mod = static_cast<ColourModule*>(module);

    this->scheduleUpdate();

    auto label = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110, 0.575f, 0);

    colourSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    colourSpr->setScale(0.65f);

    colourBtn = CCMenuItemSpriteExtra::create(colourSpr, this, menu_selector(ColourModuleNode::onChangeColour));

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(colourBtn, Anchor::Left, ccp(label->getScaledContentWidth() + 25, 0));
}

void ColourModuleNode::updateNode()
{
    
}

void ColourModuleNode::onChangeColour(CCObject* sender)
{
    auto mod = static_cast<ColourModule*>(module);

    auto ui = SetupColourConfigUI::create([this, mod](ColourConfig conf)
    {
        mod->setConfig(conf);
    });

    ui->setStartConfig(mod->getConfig());
    ui->setDefaultConfig(mod->getDefaultConfig());
    ui->setPreviewChannel(mod->getID());

    ui->show();
}