#include "BetterAlertLayer.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "BetterButtonSprite.hpp"
#include "EasyBG.hpp"

BetterAlertLayer* BetterAlertLayer::create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
{
    auto pRet = new BetterAlertLayer();

    pRet->delegate = delegate;
    pRet->title = title;
    pRet->desc = desc;
    pRet->btn1 = btn1;
    pRet->btn2 = btn2;
    pRet->width = width;
    pRet->scroll = scroll;
    pRet->height = height;
    pRet->textScale = textScale;

    if (pRet && pRet->init(0, 0))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

BetterAlertLayer* BetterAlertLayer::create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width)
{
    return BetterAlertLayer::create(delegate, title, desc, btn1, btn2, width, false, 0.0f, 1.0f);
}

BetterAlertLayer* BetterAlertLayer::create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2)
{
    return BetterAlertLayer::create(delegate, title, desc, btn1, btn2, 300.0);
}

BetterAlertLayer* BetterAlertLayer::create(char const* title, const gd::string& desc, char const* btn)
{
    return BetterAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0);
}

BetterAlertLayer* BetterAlertLayer::createWithLocalisation(char const* title, const gd::string& desc, char const* btn)
{
    auto lm = LocalisationManager::get();

    return create(lm->getLocalisedString(title).c_str(), lm->getLocalisedString(desc), lm->getLocalisedString(btn).c_str());
}

bool BetterAlertLayer::setup()
{
    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    content = AdvLabelBMFont::createWithStruct({}, "chatFont.fnt");
    content->setMaxWidth(width - 60);
    content->setSplitEverySpace(true);
    content->setLineSpacing(3.5f);
    content->setAlignment(kCCTextAlignmentCenter);
    content->setString(desc.c_str());

    float desHeight = std::max<float>(140.0, content->getScaledContentHeight() + 100);

    m_mainLayer->setContentSize(ccp(width, desHeight));

    bg = CCScale9Sprite::create("square01_001.png");
    bg->setContentSize(ccp(width, desHeight));

    titleLabel = AdvLabelBMFont::createWithString(title, "goldFont.fnt");
    titleLabel->setAnchorPoint(ccp(0.5f, 1));
    titleLabel->limitLabelWidth(width - 60, 0.9f, 0);

    auto menu = CCMenu::create();
    menu->setZOrder(8);

    auto spr = BetterButtonSprite::create(ccp(54.25f, 30), btn1, "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(BetterAlertLayer::onButton));
    btn->setTag(1);
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(bg, Anchor::Center);
    m_mainLayer->addChildAtPosition(content, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(titleLabel, Anchor::Top, ccp(0, -15));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 30));
    return true;
}

void BetterAlertLayer::onButton(CCObject* sender)
{
    if (sender->getTag() == 1)
    {
        this->onBtn1(sender);
    }
    else
    {
        this->onBtn2(sender);
    }
}