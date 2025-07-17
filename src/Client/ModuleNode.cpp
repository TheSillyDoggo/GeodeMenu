#include "ModuleNode.hpp"
#include "../GUI/Categories/FavouritesNode.hpp"

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

    btn = CCMenuItemToggler::createWithStandardSprites(this, isDisabled ? menu_selector(ModuleNode::onToggleError) : menu_selector(ModuleNode::onToggle), 0.75f);
    btn->toggle(module->getUserEnabled());

    auto label = CCLabelBMFont::create(module->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110 - (hasDesc ? 0 : 0), 0.575f, 0.1f);

    favBtn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), this, menu_selector(ModuleNode::onToggleFavourite));
    favBtn->setScale(0.65f);
    favBtn->toggle(module->isFavourited());
    favBtn->setVisible(false);

    favBtn->m_offButton->setColor(ccc3(150, 150, 150));
    favBtn->m_offButton->setOpacity(150);

    if (isDisabled)
    {
        label->setOpacity(150);
        btn->m_onButton->setOpacity(150);
        btn->m_offButton->setOpacity(150);
    }

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(label, Anchor::Left, ccp(30, 0));
    this->addChildAtPosition(favBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, -8));

    if (hasDesc)
    {
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.55f + 0.35f);

        auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModuleNode::onInfo));

        infoSpr->setScale(infoSpr->getScale() - 0.35f);

        this->addChildAtPosition(infoBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, 5));
    }
}

void ModuleNode::updateNode()
{
    if (btn->isToggled() != module->getUserEnabled())
        btn->toggle(module->getUserEnabled());

    if (favBtn->isToggled() != module->isFavourited())
        favBtn->toggle(module->isFavourited());
}

void ModuleNode::onToggle(CCObject* sender)
{
    module->setUserEnabled(!module->getUserEnabled());
    updateAllNodes(this);

    module->onToggle();
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

void ModuleNode::onToggleFavourite(CCObject* sender)
{
    module->setFavourited(!module->isFavourited());
    updateAllNodes(this);

    FavouritesNode::get()->refresh();
}

void ModuleNode::onInfoToggleFavourite(CCObject* sender)
{
    module->setFavourited(!module->isFavourited());

    FavouritesNode::get()->refresh();
}

void ModuleNode::onInfo(CCObject* sender)
{
    auto alert = FLAlertLayer::create(module->getName().c_str(), module->getDescription(), "OK");
    alert->show();

    auto menu = CCMenu::create();
    // この二行は怖いだ
    menu->setPosition(CCDirector::get()->getWinSize() / 2 - (alert->m_mainLayer->getChildByType<CCScale9Sprite>(0)->getContentSize() / 2) + ccp(25, 25));
    menu->setTouchPriority(-42069);

    auto btn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), this, menu_selector(ModuleNode::onInfoToggleFavourite));
    btn->toggle(module->isFavourited());

    btn->setContentSize(btn->getContentSize() * 3);

    btn->m_offButton->setContentSize(btn->getContentSize());
    btn->m_offButton->setPosition(btn->getContentSize() / 2);
    btn->m_offButton->getNormalImage()->setPosition(btn->getContentSize() / 2);
    
    btn->m_onButton->setContentSize(btn->getContentSize());
    btn->m_onButton->setPosition(btn->getContentSize() / 2);
    btn->m_onButton->getNormalImage()->setPosition(btn->getContentSize() / 2);

    btn->m_offButton->setColor(ccc3(150, 150, 150));
    btn->m_offButton->setOpacity(150);

    menu->addChild(btn);
    alert->m_mainLayer->addChild(menu, 8008569);

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

    if (auto label = alert->m_mainLayer->getChildByType<CCLabelBMFont>(0))
    {
        label->limitLabelWidth(270, 0.9f, 0);
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

    nodes.push_back(this);

    return true;
}

void ModuleNode::updateAllNodes(ModuleNode* except)
{
    for (auto node : nodes)
    {
        if (node != except)
            node->updateNode();
    }
}

ModuleNode::~ModuleNode()
{
    nodes.erase(std::find(nodes.begin(), nodes.end(), this));
}