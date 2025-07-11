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
    bool hasDesc = module->getDescription().size() > 0;
    bool isDisabled = module->isDisabled();

    auto btn = CCMenuItemToggler::createWithStandardSprites(this, isDisabled ? menu_selector(ModuleNode::onToggleError) : menu_selector(ModuleNode::onToggle), 0.75f);
    btn->toggle(module->getUserEnabled());

    auto label = CCLabelBMFont::create(module->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110 - (hasDesc ? 0 : 0), 0.575f, 0.1f);

    if (isDisabled)
    {
        label->setOpacity(150);
        btn->m_onButton->setOpacity(150);
        btn->m_offButton->setOpacity(150);
    }

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(label, Anchor::Left, ccp(30, 0));

    if (hasDesc)
    {
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.55f + 0.35f);

        auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModuleNode::onInfo));

        infoSpr->setScale(infoSpr->getScale() - 0.35f);

        this->addChildAtPosition(infoBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, 5));
    }
}

void ModuleNode::onToggle(CCObject* sender)
{
    module->setUserEnabled(!module->getUserEnabled());
}

void ModuleNode::onToggleError(CCObject* sender)
{
    auto toggler = as<CCMenuItemToggler*>(sender);
    toggler->toggle(!toggler->isToggled());

    toggler->m_onButton->setScale(1);
    toggler->m_offButton->setScale(1);

    toggler->m_onButton->stopAllActions();
    toggler->m_offButton->stopAllActions();

    FLAlertLayer::create("Help", "Disabled module help text", "OK")->show();
}

void ModuleNode::onInfo(CCObject* sender)
{
    auto alert = FLAlertLayer::create(module->getName().c_str(), module->getDescription(), "OK");
    alert->show();

    // :3
    if (module->getID() == "no-trans")
    {
        if (auto textArea = alert->m_mainLayer->getChildByType<TextArea>(0))
        {
            auto clip = CCClippingNode::create(textArea);
            clip->setAlphaThreshold(0.03f);

            auto trans = CCScale9Sprite::create("geode.loader/GE_button_04.png");
            trans->_scale9Image->setBlendFunc({GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
            trans->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(25.5f, 15));
            trans->setContentWidth(66);
            trans->setScaleY(0.375f);
            clip->addChild(trans);

            alert->m_mainLayer->addChild(clip, 80085);
        }
    }
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