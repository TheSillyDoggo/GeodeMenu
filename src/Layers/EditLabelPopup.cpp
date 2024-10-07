#include "EditLabelPopup.hpp"
#include "../Client/AndroidUI.h"

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

    auto page2 = CCMenu::create();
    page2->setPosition(ccp(0, 0));
    page2->setAnchorPoint(CCPointZero);
    page2->setContentSize(size);
    pages.push_back(page2);
    page2->setID("Format");

    auto formatSize = ccp(320, 75);
    auto nameInp = TextInput::create(140, "Display Name");
    nameInp->setString(module->name);
    nameInp->setCallback([this, nameInp](const std::string& str)
    {
        module->name = str;
        nameInp->getInputNode()->m_placeholderLabel->setScale(1);
    });

    auto formatInp = TextInput::create(formatSize.x, "Format");
    formatInp->setString(module->format);
    formatInp->setCallback([this](const std::string& str)
    {
        module->format = str;
    });

    formatInp->setPositionY(30);

    baseLayer->addChildAtPosition(leftBtn, Anchor::Left, ccp(-25, 0));
    baseLayer->addChildAtPosition(rightBtn, Anchor::Right, ccp(25, 0));
    
    baseLayer->addChild(page1);
    baseLayer->addChild(page2);

    page1->addChild(nameInp);

    page2->addChildAtPosition(formatInp, Anchor::Center);

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