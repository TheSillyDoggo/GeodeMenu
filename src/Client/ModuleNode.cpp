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

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
}

void ModuleNode::onToggle(CCObject* sender)
{
    module->setUserEnabled(!module->getUserEnabled());
}

bool ModuleNode::init(Module* module)
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(ccp(150, 28));

    this->module = module;
    setup();

    return true;
}