#include "ModuleNode.hpp"

ModuleNode* ModuleNode::create(Module* module)
{
    auto pRet = new ModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ModuleNode::setup()
{
    auto btn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ModuleNode::onToggle), 0.75f);
    btn->toggle(module->getUserEnabled());

    auto label = CCLabelBMFont::create(module->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110, 0.575f, 0.1f);

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(label, Anchor::Left, ccp(30, 0));
}

void ModuleNode::onToggle(CCObject* sender)
{
    module->setUserEnabled(!module->getUserEnabled());
}

bool ModuleNode::init(Module* module)
{
    if (!CCMenu::init())
        return false;

    this->setID(module->getID());

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(ccp(165, 28));

    this->module = module;
    setup();

    return true;
}