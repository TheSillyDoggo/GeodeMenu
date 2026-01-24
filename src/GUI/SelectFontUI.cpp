#include "SelectFontUI.hpp"

#include "BetterButtonSprite.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "BetterAlertLayer.hpp"

SelectFontUI* SelectFontUI::create(std::function<void(std::string)> onFinish, bool allowNonGDFonts)
{
    auto pRet = new SelectFontUI();

    pRet->onFinish = onFinish;
    pRet->allowNonGDFonts = allowNonGDFonts;

    if (pRet && pRet->init(360, 250))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void SelectFontUI::setDefaultFont(std::string font)
{
    this->defaultFont = font;

    updateUI();
}

void SelectFontUI::setStartFont(std::string font)
{
    this->startFont = font;
    this->currentFont = font;

    if (togglers[font])
    {
        page2 = (togglers[font]->getParent()->getParent()->getParent() == scroll2);
    }

    updateUI();
}

void SelectFontUI::onSetDefault(CCObject* sender)
{
    currentFont = defaultFont;

    updateUI();
}

void SelectFontUI::onUndoChanged(CCObject* sender)
{
    currentFont = startFont;

    updateUI();
}

void SelectFontUI::onChangeFont(CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);

    currentFont = toggler->getID();

    updateUI(toggler);
}

void SelectFontUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(currentFont);

    PopupBase::onClose(sender);
}

void SelectFontUI::onChangePage(CCObject* sender)
{
    page2 = !page2;

    updateUI();
}

void SelectFontUI::onFontHint(CCObject* sender)
{
    auto alert = BetterAlertLayer::create("Writing system", LocalisationManager::get()->getLocalisedString(fmt::format("font-select/{}", static_cast<CCNode*>(sender)->getID())), LocalisationManager::get()->getLocalisedString("ui/ok-button").c_str());
    alert->show();
}

void SelectFontUI::updateUI(CCMenuItemToggler* except)
{
    for (auto toggler : togglers)
    {
        toggler.second->setEnabled(currentFont != toggler.first);

        bool v = currentFont == toggler.first;

        toggler.second->toggle(v);
    }

    scroll1->setVisible(!page2);
    scroll2->setVisible(page2);
    scrollbar1->setVisible(!page2);
    scrollbar2->setVisible(page2);
}

void SelectFontUI::update(float dt)
{
    for (auto label : labels)
    {
        if (!label.second->getUserObject("loaded"_spr))
        {
            if (nodeIsVisible(label.second->getParent()))
            {
                label.second->setUserObject("loaded"_spr, CCBool::create(true));
                label.second->setFntFile(label.second->getID().c_str());
                label.second->setScale(20 / label.second->getContentHeight());

                auto m = getHints(label.second->getConfiguration());

                label.second->getParent()->addChildAtPosition(m, Anchor::BottomLeft, ccp(label.second->getScaledContentWidth() + 10 + 10, 0));
            }
        }
    }
}

CCMenu* SelectFontUI::getHints(CCBMFontConfiguration* config)
{
    auto menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setScale(0.4f);
    menu->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(15));

    if (config->getCharacterSet()->contains(12399)) // わ
    {
        auto l = AdvLabelBMFont::createWithString("JA", "bigFont.fnt");
        l->setOpacity(150);

        auto btn = CCMenuItemSpriteExtra::create(l, this, menu_selector(SelectFontUI::onFontHint));
        btn->setID("japanese-supported");

        menu->addChild(btn);
    }

    if (config->getCharacterSet()->contains(1047)) // з
    {
        auto l = AdvLabelBMFont::createWithString("RU", "bigFont.fnt");
        l->setOpacity(150);

        auto btn = CCMenuItemSpriteExtra::create(l, this, menu_selector(SelectFontUI::onFontHint));
        btn->setID("russian-supported");

        menu->addChild(btn);
    }

    if (config->getCharacterSet()->contains(51020)) // 음
    {
        auto l = AdvLabelBMFont::createWithString("KO", "bigFont.fnt");
        l->setOpacity(150);

        auto btn = CCMenuItemSpriteExtra::create(l, this, menu_selector(SelectFontUI::onFontHint));
        btn->setID("korean-supported");

        menu->addChild(btn);
    }

    /*if (config->getCharacterSet()->contains(31627)) // 简
    {
        auto l = AdvLabelBMFont::createWithString("ZH", "bigFont.fnt");
        l->setOpacity(150);

        auto btn = CCMenuItemSpriteExtra::create(l, this, menu_selector(SelectFontUI::onFontHint));
        btn->setID("chinese-supported");

        menu->addChild(btn);
    }*/

    menu->updateLayout();
    return menu;
}

CCMenu* SelectFontUI::createFontCell(std::string font, int index, std::string name)
{
    CCSize size = ccp(280 - 15, 32);

    auto menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setContentSize(size);
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(size);

    auto bg = CCLayerColor::create(ccc4(0, 0, 0, index % 2 == 0 ? 50 : 100));
    bg->setAnchorPoint(ccp(0, 0));
    bg->ignoreAnchorPointForPosition(false);
    bg->setContentSize(size);
    bg->setPosition(ccp(0, 0));

    auto label = AdvLabelBMFont::createWithString(name, index < 7 ? font : "bigFont.fnt");
    label->setID(font);
    label->setTag(index);
    label->setScale(20 / label->getContentHeight());
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setTTFUsage(AdvLabelTTFUsage::None);

    if (font == "gjFont01.fnt")
        label->setFntFile("gjFont01.fnt");

    if (index < 7)
        label->setUserObject("loaded"_spr, CCBool::create(true));
    else
    {
        if (CCTextureCache::get()->textureForKey(utils::string::replace(font, ".fnt", ".png").c_str()))
        {
            label->setUserObject("loaded"_spr, CCBool::create(true));
            label->setFntFile(font.c_str());
            label->setScale(20 / label->getContentHeight());

            auto m = getHints(label->getConfiguration());

            menu->addChildAtPosition(m, Anchor::BottomLeft, ccp(label->getScaledContentWidth() + 10 + 10, 2));
        }
    }

    auto toggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png"), CCSprite::createWithSpriteFrameName("GJ_selectSongOnBtn_001.png"), this, menu_selector(SelectFontUI::onChangeFont));
    toggler->setScale(0.6f);
    toggler->setID(font);

    labels.emplace(font, label);
    togglers.emplace(font, toggler);

    menu->addChild(bg);
    menu->addChildAtPosition(label, Anchor::Left, ccp(10, 0));
    menu->addChildAtPosition(toggler, Anchor::Right, -ccp(size.height / 2, 0));
    return menu;
}

bool SelectFontUI::setup()
{
    
    this->scheduleUpdate();

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("font-select/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SelectFontUI::onClose));
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));

    // 59 being the custom font count
    int fontCount = 59 + 3;
    float height = 32 * fontCount;

    scrollbar1 = BetterScrollbar::create(scroll1);
    scrollbar2 = BetterScrollbar::create(scroll2);
    scrollbar2->setDisabled(true);

    scroll1 = ScrollLayer::create(ccp(280 - 15, 160));
    scroll1->m_contentLayer->setContentHeight(height);
    scroll1->m_peekLimitTop = 15;
    scroll1->m_peekLimitBottom = 15;
    scroll1->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());

    scroll2 = ScrollLayer::create(ccp(280 - 15, 160));
    scroll2->m_peekLimitTop = 15;
    scroll2->m_peekLimitBottom = 15;
    scroll2->setMouseEnabled(false);
    scroll2->setTouchEnabled(false);
    scroll2->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());

    scrollbar1->setLayer(scroll1);
    scrollbar2->setLayer(scroll2);

    for (size_t i = 0; i < fontCount; i++)
    {
        std::string font = "";
        std::string name = "";

        if (i == 0)
        {
            font = "bigFont.fnt";
            name = "Pusab";
        }
        else if (i == 1)
        {
            font = "goldFont.fnt";
            name = "Pusab (Gold)";
        }
        else if (i == 2)
        {
            font = "chatFont.fnt";
            name = "Comment Font";
        }
        else if (i - 2 < 10)
            font = fmt::format("gjFont0{}.fnt", i - 2);
        else
            font = fmt::format("gjFont{}.fnt", i - 2);

        if (name.empty())
            name = fmt::format("Font {}", i - 2);

        auto cell = createFontCell(font, i, name);
        cell->setPosition(ccp(0, height - (cell->getContentHeight() * (i + 1))));

        scroll1->m_contentLayer->addChild(cell);
    }

    auto borders = CCLayerColor::create();
    borders->setAnchorPoint(ccp(0.5f, 0.5f));
    borders->ignoreAnchorPointForPosition(false);
    borders->setColor(ccc3(0, 0, 0));
    borders->setOpacity(50);
    borders->setContentSize(scroll1->getContentSize() + ccp(2, 2));

    auto pageMenu = CCMenu::create();
    pageMenu->setVisible(this->allowNonGDFonts);
    pageMenu->setAnchorPoint(ccp(0, 0));
    pageMenu->setScale(0.7f);

    auto btnRight = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(SelectFontUI::onChangePage));
    btnRight->getNormalImage()->setScaleX(-1);
    auto btnLeft = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(SelectFontUI::onChangePage));

    btnLeft->setPosition(ccp(-155 / pageMenu->getScale(), 0));
    btnRight->setPosition(ccp(155 / pageMenu->getScale(), 0));

    pageMenu->addChild(btnLeft);
    pageMenu->addChild(btnRight);


    height = scroll2->getContentHeight();

    auto cell = createFontCell("kosugimaru.fnt"_spr, 10, "Kosugi Maru");
    cell->setPosition(ccp(0, height - (cell->getContentHeight() * 1)));

    scroll2->m_contentLayer->addChild(cell);

    cell = createFontCell("lexend.fnt"_spr, 11, "Lexend");
    cell->setPosition(ccp(0, height - (cell->getContentHeight() * 2)));

    scroll2->m_contentLayer->addChild(cell);

    cell = createFontCell("notosans.fnt"_spr, 12, "Noto Sans");
    cell->setPosition(ccp(0, height - (cell->getContentHeight() * 3)));
    
    scroll2->m_contentLayer->addChild(cell);

    scroll1->moveToTop();
    scroll2->moveToTop();
    m_mainLayer->addChildAtPosition(borders, Anchor::Center, ccp(-15 / 2, 5));
    m_mainLayer->addChildAtPosition(pageMenu, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(scroll1, Anchor::Center, scroll1->getContentSize() * -0.5f + ccp(-15 / 2, 5));
    m_mainLayer->addChildAtPosition(scroll2, Anchor::Center, scroll2->getContentSize() * -0.5f + ccp(-15 / 2, 5));
    m_mainLayer->addChildAtPosition(scrollbar1, Anchor::Center, ccp((scroll1->getContentWidth() / 2), 5));
    m_mainLayer->addChildAtPosition(scrollbar2, Anchor::Center, ccp((scroll2->getContentWidth() / 2), 5));
    return true;
}