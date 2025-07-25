#include "OptionsUI.hpp"
#include "Categories/FavouritesNode.hpp"

OptionsUI* OptionsUI::create(Module* mod)
{
    auto pRet = new OptionsUI();

    pRet->module = mod;
    auto size = pRet->calculateSize();

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

CCSize OptionsUI::calculateSize()
{
    // 340 is the width of a category node
    return ccp(340 + 20, 250);
}

bool OptionsUI::setup()
{
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = CCLabelBMFont::create(module->getName().c_str(), "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();
    auto menu2 = CCMenu::create();
    auto menu3 = CCMenu::create();

    auto btn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(OptionsUI::onClose));
    menu->addChild(btn);

    auto infoBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(OptionsUI::onInfo));
    infoBtn->getNormalImage()->setScale(0.75f);
    menu2->addChild(infoBtn);

    favBtn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), this, menu_selector(OptionsUI::onToggleFavourite));
    favBtn->toggle(module->isFavourited());

    favBtn->setContentSize(btn->getContentSize() * 3);

    favBtn->m_offButton->setContentSize(favBtn->getContentSize());
    favBtn->m_offButton->setPosition(favBtn->getContentSize() / 2);
    favBtn->m_offButton->getNormalImage()->setPosition(favBtn->getContentSize() / 2);
    
    favBtn->m_onButton->setContentSize(favBtn->getContentSize());
    favBtn->m_onButton->setPosition(favBtn->getContentSize() / 2);
    favBtn->m_onButton->getNormalImage()->setPosition(favBtn->getContentSize() / 2);

    favBtn->m_offButton->setColor(ccc3(150, 150, 150));
    favBtn->m_offButton->setOpacity(150);

    menu3->addChild(favBtn);

    node = CategoryNode::create();
    node->setAnchorPoint(ccp(0.5f, 0.5f));
    node->setContentHeight(170);
    node->updateLayout();

    for (auto option : module->getOptions())
    {
        node->addModule(option);
    }

    m_mainLayer->addChildAtPosition(node, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(menu2, Anchor::TopRight, ccp(-16, -18));
    m_mainLayer->addChildAtPosition(menu3, Anchor::TopLeft, ccp(16, -18));

    return true;
}

void OptionsUI::onInfo(CCObject* sender)
{
    auto alert = FLAlertLayer::create(module->getName().c_str(), module->getDescription(), "OK");
    alert->setUserData(module);
    alert->setUserObject("fav-btn", favBtn);
    alert->show();

    auto menu = CCMenu::create();
    // この二行は怖いだ
    menu->setPosition(CCDirector::get()->getWinSize() / 2 - (alert->m_mainLayer->getChildByType<CCScale9Sprite>(0)->getContentSize() / 2) + ccp(25, 25));
    menu->setTouchPriority(-42069);

    auto btn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), alert, menu_selector(OptionsUI::onInfoToggleFavourite));
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

    // title
    if (auto label = alert->m_mainLayer->getChildByType<CCLabelBMFont>(0))
    {
        label->limitLabelWidth(270, 0.9f, 0);
    }
}

void OptionsUI::onToggleFavourite(CCObject* sender)
{
    module->setFavourited(!module->isFavourited());
    
    if (FavouritesNode::get())
        FavouritesNode::get()->refresh();
}

void OptionsUI::onInfoToggleFavourite(CCObject* sender)
{
    // 'this' is the alert in the context of this function

    auto mod = as<Module*>(this->getUserData());
    auto favBtn = as<CCMenuItemToggler*>(this->getUserObject("fav-btn"));

    favBtn->toggle(!mod->isFavourited());
    mod->setFavourited(!mod->isFavourited());

    if (FavouritesNode::get())
        FavouritesNode::get()->refresh();
}
