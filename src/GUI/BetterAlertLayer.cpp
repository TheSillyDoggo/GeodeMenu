#include "BetterAlertLayer.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "BetterButtonSprite.hpp"

BetterAlertLayer* BetterAlertLayer::create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
{
    auto pRet = new BetterAlertLayer();

    if (pRet && pRet->init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale))
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

bool BetterAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
{
    content = AdvLabelBMFont::createWithStruct({}, "chatFont.fnt");
    content->setMaxWidth(width - 30);
    content->setSplitEverySpace(true);
    content->setAlignment(kCCTextAlignmentCenter);
    content->setString(desc.c_str());

    std::string ss;

    for (size_t i = 0; i < content->getLineCount(); i++)
    {
        ss.append("boobs\n");
    }

    if (!FLAlertLayer::init(delegate, title, ss, btn1, btn2, width, scroll, height, textScale))
        return false;

    auto oldTitle = m_mainLayer->getChildByType<CCLabelBMFont>(0);
    auto area = m_mainLayer->getChildByType<TextArea>(0);
    auto bg = m_mainLayer->getChildByType<CCScale9Sprite>(0);
    auto men = m_mainLayer->getChildByType<CCMenu>(0);

    oldTitle->setVisible(false);
    area->setVisible(false);
    men->setVisible(false);

    titleLabel = AdvLabelBMFont::createWithString(title, "goldFont.fnt");
    titleLabel->setPosition(ccp(getContentWidth() / 2, getContentHeight() / 2 + bg->getScaledContentHeight() / 2 + -15));
    titleLabel->setAnchorPoint(ccp(0.5f, 1));
    titleLabel->limitLabelWidth(width - 30, 0.9f, 0);

    content->setPosition(ccp(getContentWidth() / 2, getContentHeight() / 2 + 5));

    auto menu = CCMenu::create();
    menu->setTouchPriority(-512);
    menu->setPosition(ccp(getContentWidth() / 2, getContentHeight() / 2 - bg->getScaledContentHeight() / 2 + 30));

    auto spr = BetterButtonSprite::create(ccp(54.25f, 30), btn1, "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(BetterAlertLayer::onButton));
    btn->setTag(1);
    menu->addChild(btn);

    m_mainLayer->addChild(content, 6);
    m_mainLayer->addChild(titleLabel, 7);
    m_mainLayer->addChild(menu, 8);
    return true;
}

void BetterAlertLayer::onButton(CCObject* sender)
{
    if (sender->getTag() == 1)
    {
        this->onBtn1(m_button1->getParent());
    }
    else
    {
        this->onBtn2(m_button2->getParent());
    }
}