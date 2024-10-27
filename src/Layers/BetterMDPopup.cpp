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

    std::unordered_map<float, std::vector<CCNode*>> posMap;
    std::unordered_map<float, float> widthMap;

    for (auto child : CCArrayExt<CCNode*>(content->getChildren()))
    {
        posMap[child->getPositionY()].push_back(child);
        widthMap[child->getPositionY()] += child->getScaledContentWidth();
    }

    for (auto obj : posMap)
    {
        for (auto child : obj.second)
        {
            child->setPositionX(md->getContentWidth() / 2 + child->getPositionX() - widthMap[obj.first] / 2);
        }
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

BetterMDPopup* BetterMDPopup::create(char const* title, gd::string desc, char const* btn1, char const* btn2)
{
    return BetterMDPopup::create(nullptr, title, desc, btn1, btn2, 420, true, 69, 1.0f);
}