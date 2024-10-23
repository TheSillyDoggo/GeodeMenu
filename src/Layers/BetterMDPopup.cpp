#include "BetterMDPopup.hpp"

bool BetterMDPopup::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
{
    if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale))
        return false;

    desc = utils::string::replace(desc, "\n", "\n\n");

    m_scrollingLayer->setVisible(false);

    auto md = MDTextArea::create(desc, ccp(365, 175));
    md->setPosition(CCDirector::get()->getWinSize() / 2);

    md->setTouchEnabled(true);

    auto content = public_cast(md, m_content);

    for (auto child : CCArrayExt<CCNode*>(content->getChildren()))
    {
        //child->setPositionX(child->getPositionX() + content->getContentWidth() / 2);
    }

    m_mainLayer->addChild(md, 69);

    handleTouchPriority(this);

    return true;
}

BetterMDPopup* BetterMDPopup::create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
{
    auto pRet = new BetterMDPopup();

    if (pRet && pRet->init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}