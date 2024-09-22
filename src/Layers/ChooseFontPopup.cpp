#include "ChooseFontPopup.hpp"

void ChooseFontPopup::customSetup()
{
    // 59 being the custom song count
    int fontCount = 59 + 3;
    float cellSize = 30;

    auto border = geode::ListBorders::create();
    border->setContentSize(ccp(320, 210));
    border->setZOrder(69);
    border->setSpriteFrames("geode.loader/geode-list-top.png", "geode.loader/geode-list-side.png", 2.25f);
    
    for (auto child : CCArrayExt<CCNodeRGBA*>(border->getChildren()))
    {
        child->setColor(ccc3(0, 0, 0));
    }

    auto filter = CCScale9Sprite::create("GJ_square01.png");
    filter->setContentSize((border->getContentSize() + ccp(0, 5)) * 2);
    filter->setScale(1.0f / 2.0f);
    auto clipping = CCClippingNode::create(filter);
    clipping->setAlphaThreshold(0.01f);

    auto scroll = ScrollLayer::create(filter->getContentSize() / 2);
    scroll->m_contentLayer->setContentHeight(cellSize * fontCount);
    scroll->moveToTop();
    scroll->m_contentLayer->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setGap(0)->setAxisReverse(true));

    for (size_t i = 0; i < fontCount; i++)
    {
        std::string name;
        std::string font;

        name = fmt::format("Font {}", i + 1 - 3);

        if (i == 0)
            name = "Pusab";
        else if (i == 1)
            name = "Pusab (Gold)";
        else if (i == 2)
            name = "Comment Font";

        if (i == 0)
            font = "bigFont.fnt";
        else if (i == 1)
            font = "goldFont.fnt";
        else if (i == 2)
            font = "chatFont.fnt";
        else if (i - 2 < 10)
            font = fmt::format("gjFont0{}.fnt", i - 2);
        else
            font = fmt::format("gjFont{}.fnt", i - 2);

        auto cell = CCLayerColor::create(ccc4(0, 0, 0, i % 2 ? 150 : 75));
        cell->ignoreAnchorPointForPosition(false);
        cell->setContentWidth(320);
        cell->setContentHeight(cellSize);
        cell->setAnchorPoint(ccp(0, 1));

        auto menu = CCMenu::create();
        menu->setAnchorPoint(ccp(0, 0));
        menu->setScale(0.6f);

        auto btn = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png"), CCSprite::createWithSpriteFrameName("GJ_selectSongOnBtn_001.png"), this, menu_selector(ChooseFontPopup::onSelect));
        btn->setTag(i);
        btn->setID(font);
        menu->addChild(btn);

        toggles.push_back(btn);

        auto lbl = CCLabelBMFont::create(name.c_str(), font.c_str());
        lbl->setAnchorPoint(ccp(0, 0.5f));
        lbl->setScale(20 / lbl->getContentHeight());

        cell->addChildAtPosition(menu, Anchor::Right, ccp(-17.5f, 0));
        cell->addChildAtPosition(lbl, Anchor::Left, ccp(6, 0));
        scroll->m_contentLayer->addChild(cell);
    }

    scroll->m_contentLayer->updateLayout();
    clipping->addChildAtPosition(scroll, Anchor::BottomLeft, -scroll->getContentSize() / 2);
    baseLayer->addChildAtPosition(clipping, Anchor::Center, ccp(0, 5));
    baseLayer->addChildAtPosition(border, Anchor::Center, ccp(0, 5));

    setSelected(0);
}

void ChooseFontPopup::setSelected(int id)
{
    for (size_t i = 0; i < toggles.size(); i++)
    {
        toggles[i]->setEnabled(i != id);
        toggles[i]->toggle(i == id);
    }
}

void ChooseFontPopup::setSelected(std::string id)
{
    for (size_t i = 0; i < toggles.size(); i++)
    {
        if (toggles[i]->getID() == id)
        {
            setSelected(i);
            return;
        }
    }

    setSelected(0);
}

void ChooseFontPopup::onSelect(CCObject* sender)
{
    setSelected(sender->getTag());

    if (callback)
        callback(as<CCNode*>(sender)->getID());
}

ChooseFontPopup* ChooseFontPopup::create(geode::utils::MiniFunction<void(std::string)> callback)
{
    auto pRet = new ChooseFontPopup();

    pRet->callback = callback;

    if (pRet && pRet->initWithSizeAndName(ccp(400, 290), "Choose Font"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

ChooseFontPopup* ChooseFontPopup::addToScene(geode::utils::MiniFunction<void(std::string)> callback)
{
    auto pRet = ChooseFontPopup::create(callback);

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}