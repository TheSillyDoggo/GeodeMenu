#include "SelectItemUI.hpp"
#include "BetterButtonSprite.hpp"


SelectItemUI* SelectItemUI::create(SelectItemType type, std::function<void(std::string)> onFinish)
{
    auto pRet = new SelectItemUI();

    CCSize size = ccp(350, 230);
    pRet->type = type;
    pRet->onFinish = onFinish;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void SelectItemUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(selectedItem);

    PopupBase::onClose(sender);
}

void SelectItemUI::onSelectItem(CCObject* sender)
{
    setSelectedItem(static_cast<CCNode*>(sender)->getID());
}

void SelectItemUI::setSelectedItem(std::string item)
{
    this->selectedItem = item;

    if (buttons.contains(item))
        selectedOutline->setPosition(buttons[item]->getPosition());
    else
        selectedOutline->setPosition(ccp(-1000, -1000));

    for (auto btn : buttons)
    {
        btn.second->setEnabled(btn.first != item);
    }
}

void SelectItemUI::updateUI()
{
    std::vector<std::string> items = {};
    bool useSpriteFrame = false;

    switch (type)
    {
        case SelectItemType::ShortcutBG:
            useSpriteFrame = true;
            items.push_back("");
            items.push_back("geode.loader/baseCircle_Medium_Blue.png");
            items.push_back("geode.loader/baseCircle_Medium_Cyan.png");
            items.push_back("geode.loader/baseCircle_Medium_DarkAqua.png");
            items.push_back("geode.loader/baseCircle_Medium_DarkPurple.png");
            items.push_back("geode.loader/baseCircle_Medium_Gray.png");
            items.push_back("geode.loader/baseCircle_Medium_Green.png");
            items.push_back("geode.loader/baseCircle_Medium_Pink.png");
            items.push_back("geode1.png"_spr);
            items.push_back("geode2.png"_spr);
            items.push_back("geode3.png"_spr);
            break;

        case SelectItemType::ShortcutOverlay:
            items.push_back("");
            items.push_back("button.png"_spr);
            items.push_back("config.png"_spr);
            items.push_back("cosmetic.png"_spr);
            items.push_back("creator.png"_spr);
            items.push_back("favourites.png"_spr);
            items.push_back("format.png"_spr);
            items.push_back("icon effects.png"_spr);
            items.push_back("keybinds.png"_spr);
            items.push_back("labels.png"_spr);
            items.push_back("language.png"_spr);
            items.push_back("level.png"_spr);
            items.push_back("menu.png"_spr);
            items.push_back("profiles.png"_spr);
            items.push_back("rulesets.png"_spr);
            items.push_back("search.png"_spr);
            items.push_back("shortcuts.png"_spr);
            items.push_back("speedhack.png"_spr);
            items.push_back("theme.png"_spr);
            items.push_back("universal.png"_spr);
            items.push_back("preview.png"_spr);
            items.push_back("restart.png"_spr);
            items.push_back("practice-checkpoint.png"_spr);
            items.push_back("settings-cog.png"_spr);
            items.push_back("speaker.png"_spr);
            break;
    }

    for (auto item : items)
    {
        auto spr = CCNode::create();
        spr->setContentSize(ccp(17, 17));
        CCSprite* spr2 = nullptr;

        if (item.empty())
            spr2 = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
        else
        {
            if (useSpriteFrame)
                spr2 = CCSprite::createWithSpriteFrameName(item.c_str());
            else
                spr2 = CCSprite::create(item.c_str());
        }

        if (spr2)
        {
            spr2->setPosition(spr->getContentSize() / 2);
            spr2->setScale(spr->getContentWidth() / std::max<float>(spr2->getContentWidth(), spr2->getContentHeight()));
            spr->addChild(spr2);
        }

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SelectItemUI::onSelectItem));
        btn->setID(item);

        buttonsMenu->addChild(btn);
        buttons.emplace(item, btn);
    }

    buttonsMenu->updateLayout();
}

bool SelectItemUI::setup()
{
    

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    std::string titleStr = "select-item/title-generic";

    switch (type)
    {
        case SelectItemType::ShortcutBG:
            titleStr = "select-item/title-background";
            break;

        case SelectItemType::ShortcutOverlay:
            titleStr = "select-item/title-icon";
            break;
    }

    auto title = AdvLabelBMFont::createWithLocalisation(titleStr, "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();
    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SelectItemUI::onClose));
    menu->addChild(btn);

    selectedOutline = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    selectedOutline->setScale(0.7f);
    selectedOutline->setZOrder(-5);

    auto itemsBG = EasyBG::create();
    itemsBG->setContentSize(m_size + ccp(-50, -90));

    buttonsMenu = CCMenu::create();
    buttonsMenu->setAnchorPoint(ccp(0.5f, 1));
    buttonsMenu->setContentSize(itemsBG->getContentSize() + ccp(-15, -5));
    buttonsMenu->ignoreAnchorPointForPosition(false);
    buttonsMenu->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setCrossAxisAlignment(AxisAlignment::End)->setGrowCrossAxis(true)->setGap(8));

    updateUI();

    buttonsMenu->addChild(selectedOutline);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(itemsBG, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(buttonsMenu, Anchor::Top, ccp(0, -45 - 2.5f));
    return true;
}