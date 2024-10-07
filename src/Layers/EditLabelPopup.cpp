#include "EditLabelPopup.hpp"
#include "../Client/AndroidUI.h"
#include "ChooseFontPopup.hpp"

// static ButtonSprite* create(char const* caption, int width, int p2, float scale, bool absolute, char const* font, char const* bg, float height);
#define ANCHOR_BTN(__anchor, __text) \
off = ButtonSprite::create(__text, 30, 69, 1.0f, false, "bigFont.fnt", "GJ_button_04.png", 30); \
off->setContentWidth(30); \
off->m_BGSprite->setPositionX(15); \
off->m_BGSprite->setContentWidth(30); \
off->m_label->setPositionX(15); \
off->m_label->setScale(0.4f); \
on = ButtonSprite::create(__text, 30, 69, 1.0f, false, "bigFont.fnt", "GJ_button_01.png", 30); \
on->setContentWidth(30); \
on->m_BGSprite->setPositionX(15); \
on->m_BGSprite->setContentWidth(30); \
on->m_label->setPositionX(15); \
on->m_label->setScale(0.4f); \
toggler = CCMenuItemToggler::create(off, on, this, menu_selector(EditLabelPopup::onChangeAnchor)); \
toggler->setEnabled(module->getSide() != LabelAnchor::__anchor); \
toggler->toggle(module->getSide() == LabelAnchor::__anchor); \
toggler->setTag(as<int>(LabelAnchor::__anchor)); \
toggles.emplace(LabelAnchor::__anchor, toggler); \
anchorMenu->addChild(toggler);

void EditLabelPopup::customSetup()
{
    auto leftBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(EditLabelPopup::onPage));
    leftBtn->setTag(-1);

    auto rightBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(EditLabelPopup::onPage));
    rightBtn->getNormalImage()->setScaleX(-1);
    rightBtn->setTag(1);

    auto page1 = CCMenu::create();
    page1->setPosition(ccp(0, 0));
    page1->setAnchorPoint(CCPointZero);
    page1->setContentSize(size);
    pages.push_back(page1);
    page1->setID("General");

    auto anchorBG = CCScale9Sprite::create("square02_001.png");
    anchorBG->setOpacity(100);
    anchorBG->setContentSize(ccp(110, 175));
    page1->addChildAtPosition(anchorBG, Anchor::Right, ccp(-70, 0));

    auto anchorTitle = CCLabelBMFont::create("Anchor", "bigFont.fnt");
    anchorTitle->setScale(0.4f);
    anchorBG->addChildAtPosition(anchorTitle, Anchor::Top, ccp(0, -9));

    auto anchorLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    anchorLine->setRotation(90);
    anchorLine->setScaleX(0.5f);
    anchorBG->addChildAtPosition(anchorLine, Anchor::Top, ccp(0, -19));

    auto offsetTitle = CCLabelBMFont::create("Offset", "bigFont.fnt");
    offsetTitle->setScale(0.4f);
    anchorBG->addChildAtPosition(offsetTitle, Anchor::Center, ccp(0, -50));

    auto offsetLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    offsetLine->setRotation(90);
    offsetLine->setScaleX(0.5f);
    anchorBG->addChildAtPosition(offsetLine, Anchor::Center, ccp(0, -60));

    auto xInp = TextInput::create(45 / 0.7f, "X");
    xInp->setString(fmt::format("{}", module->offset.x));
    xInp->setScale(0.7f);
    xInp->setCommonFilter(CommonFilter::Float);
    xInp->setAnchorPoint(ccp(0, 0));
    xInp->setCallback([this, xInp](const std::string& str)
    {
        module->offset.x = numFromString<float>(str).unwrapOr(module->offset.x);
    });

    auto yInp = TextInput::create(45 / 0.7f, "Y");
    yInp->setString(fmt::format("{}", module->offset.y));
    yInp->setScale(0.7f);
    yInp->setCommonFilter(CommonFilter::Float);
    yInp->setAnchorPoint(ccp(1, 0));
    yInp->setCallback([this, yInp](const std::string& str)
    {
        module->offset.y = numFromString<float>(str).unwrapOr(module->offset.y);
    });

    anchorBG->addChildAtPosition(xInp, Anchor::BottomLeft, ccp(7.5f, 2.5f));
    anchorBG->addChildAtPosition(yInp, Anchor::BottomRight, ccp(-7.5f, 2.5f));

    auto anchorMenu = CCMenu::create();
    anchorMenu->setContentSize(ccp(110, 110));
    anchorMenu->ignoreAnchorPointForPosition(false);
    anchorMenu->setLayout(AxisLayout::create()->setAutoScale(false)->setGrowCrossAxis(true));

    ButtonSprite* off;
    ButtonSprite* on;
    CCMenuItemToggler* toggler;

    ANCHOR_BTN(TopLeft,      "TL");
    ANCHOR_BTN(TopCenter,    "T");
    ANCHOR_BTN(TopRight,     "TR");
    ANCHOR_BTN(CenterLeft,   "L");
    ANCHOR_BTN(Center,       "C");
    ANCHOR_BTN(CenterRight,  "R");
    ANCHOR_BTN(BottomLeft,   "BL");
    ANCHOR_BTN(BottomCenter, "B");
    ANCHOR_BTN(BottomRight,  "BR");

    anchorMenu->updateLayout();
    anchorBG->addChildAtPosition(anchorMenu, Anchor::Center, ccp(0, 13));

    auto generalBG = CCScale9Sprite::create("square02_001.png");
    generalBG->setOpacity(100);
    generalBG->setContentSize(ccp(110, 175));
    page1->addChildAtPosition(generalBG, Anchor::Left, ccp(70, 0));

    auto nameTitle = CCLabelBMFont::create("Display Name", "bigFont.fnt");
    nameTitle->setScale(0.4f);
    generalBG->addChildAtPosition(nameTitle, Anchor::Top, ccp(0, -9));

    auto nameLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    nameLine->setRotation(90);
    nameLine->setScaleX(0.5f);
    generalBG->addChildAtPosition(nameLine, Anchor::Top, ccp(0, -19));

    auto nameInp = TextInput::create(140, "Display Name");
    nameInp->setString(module->name);
    nameInp->setScale(0.7f);
    nameInp->setCallback([this, nameInp](const std::string& str)
    {
        module->name = str;
    });

    generalBG->addChildAtPosition(nameInp, Anchor::Top, ccp(0, -35));

    auto scaleTitle = CCLabelBMFont::create("Scale", "bigFont.fnt");
    scaleTitle->setScale(0.4f);
    generalBG->addChildAtPosition(scaleTitle, Anchor::Top, ccp(0, -9 - 47));

    auto scaleLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    scaleLine->setRotation(90);
    scaleLine->setScaleX(0.5f);
    generalBG->addChildAtPosition(scaleLine, Anchor::Top, ccp(0, -19 - 47));

    auto scaleInp = TextInput::create(140, "Scale");
    scaleInp->setString(fmt::format("{:.02f}", module->getScale()));
    scaleInp->setScale(0.7f);
    scaleInp->setCallback([this, scaleInp](const std::string& str)
    {
        module->setScale(numFromString<float>(str).unwrapOr(module->getScale()));
    });

    generalBG->addChildAtPosition(scaleInp, Anchor::Top, ccp(0, -35 - 47));

    auto opacityTitle = CCLabelBMFont::create("Opacity", "bigFont.fnt");
    opacityTitle->setScale(0.4f);
    generalBG->addChildAtPosition(opacityTitle, Anchor::Top, ccp(0, -9 - 47 - 47));

    auto opacityLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    opacityLine->setRotation(90);
    opacityLine->setScaleX(0.5f);
    generalBG->addChildAtPosition(opacityLine, Anchor::Top, ccp(0, -19 - 47 - 47));

    auto opacityInp = TextInput::create(140, "Scale");
    opacityInp->setString(fmt::format("{:.02f}", module->getOpacity()));
    opacityInp->setScale(0.7f);
    opacityInp->setCallback([this, opacityInp](const std::string& str)
    {
        module->setOpacity(numFromString<float>(str).unwrapOr(module->getOpacity()));
    });

    generalBG->addChildAtPosition(opacityInp, Anchor::Top, ccp(0, -35 - 47 - 47));

    auto fontMenu = CCMenu::create();
    fontMenu->setContentSize(CCPointZero);
    
    auto fontBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Change Font", 100, 0, 1.0f, false, "bigFont.fnt", "GJ_button_05.png", 23), this, menu_selector(EditLabelPopup::onFont));
    fontMenu->addChild(fontBtn);
    fontMenu->setScale(0.875f);

    generalBG->addChildAtPosition(fontMenu, Anchor::Bottom, ccp(0, 15));

    auto infoMenu = CCMenu::create();
    infoMenu->setPosition(CCPointZero);

    auto anchorInfo = InfoAlertButton::create("Anchor Info", "Anchor is the <cc>corner</c> of the screen that <cl>labels</c> are anchored to", 0.4f);
    anchorInfo->setPosition(ccp(333, 208));

    auto offsetInfo = InfoAlertButton::create("Offset Info", "Offset in <cl>cocos units</c> from the anchor point, will not affect other labels in layout", 0.4f);
    offsetInfo->setPosition(ccp(330, 78));

    auto nameInfo = InfoAlertButton::create("Display Name", "A name for your label, only shown in the <cc>ui</c> to differentiate your labels", 0.4f);
    nameInfo->setPosition(ccp(122, 208));

    infoMenu->addChild(anchorInfo);
    infoMenu->addChild(offsetInfo);
    infoMenu->addChild(nameInfo);

    page1->addChild(infoMenu);

    auto page2 = CCMenu::create();
    page2->setPosition(ccp(0, 0));
    page2->setAnchorPoint(CCPointZero);
    page2->setContentSize(size);
    pages.push_back(page2);
    page2->setID("Format");

    infoMenu = CCMenu::create();
    infoMenu->setPosition(CCPointZero);

    auto formatInfo = InfoAlertButton::create("Rift Format Help", "This is the format help", 0.8f);
    formatInfo->setPosition(size + ccp(-16, -16));

    infoMenu->addChild(formatInfo);

    auto formatSize = ccp(320, 75);

    auto formatInp = TextInput::create(formatSize.x, "Format");
    formatInp->setString(module->format);
    formatInp->setCallback([this](const std::string& str)
    {
        module->format = str;
    });

    baseLayer->addChildAtPosition(leftBtn, Anchor::Left, ccp(-25, 0));
    baseLayer->addChildAtPosition(rightBtn, Anchor::Right, ccp(25, 0));
    
    baseLayer->addChild(page1);
    baseLayer->addChild(page2);

    page2->addChildAtPosition(formatInp, Anchor::Center, ccp(0, 0));
    page2->addChild(infoMenu);

    updatePage();
    ok->m_pfnSelector = menu_selector(EditLabelPopup::onClose);
}

void EditLabelPopup::updatePage()
{
    for (size_t i = 0; i < pages.size(); i++)
    {
        pages[i]->setVisible(i == currentPage);

        if (i == currentPage)
            title->setString(pages[i]->getID().c_str());
    }
}

void EditLabelPopup::onFont(CCObject* sender)
{
    ChooseFontPopup::addToScene([this](std::string font)
    {
        log::info("selected font: {}", font);

        module->setFont(font);
    })->setSelected(module->getFont());
}

void EditLabelPopup::onChangeAnchor(CCObject* sender)
{
    module->setSide(as<LabelAnchor>(sender->getTag()));

    for (auto toggle : toggles)
    {
        toggle.second->setEnabled(toggle.first != as<LabelAnchor>(sender->getTag()));
        toggle.second->toggle(toggle.first == as<LabelAnchor>(sender->getTag()));
    }
}

void EditLabelPopup::onPage(CCObject* sender)
{
    currentPage += sender->getTag();
    
    if (currentPage < 0)
        currentPage = pages.size() - 1;

    if (currentPage > pages.size() - 1)
        currentPage = 0;

    updatePage();
}

void EditLabelPopup::onClose(CCObject* sender)
{
    if (AndroidUI::instance)
    {
        Labels::get()->refreshList();

        for (auto cell : Labels::get()->cells)
        {
            if (cell->getUserData() == module)
            {
                if (auto bg = cell->getChildByID("background"))
                {
                    as<CCScale9Sprite*>(bg)->setColor(ccc3(0, 191, 255));
                    as<CCScale9Sprite*>(bg)->runAction(CCTintTo::create(0.35f, 0, 0, 0));
                }
            }
        }
    }

    SillyBaseLayer::onClose(sender);
}

EditLabelPopup* EditLabelPopup::create(LabelModule* module, bool advanced)
{
    auto pRet = new EditLabelPopup();

    pRet->module = module;
    pRet->advanced = advanced;

    if (pRet && pRet->initWithSizeAndName(advanced ? ccp(370, 250) : ccp(290, 200), ""))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

EditLabelPopup* EditLabelPopup::addToScene(LabelModule* module, bool advanced)
{
    auto pRet = EditLabelPopup::create(module, advanced);

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}