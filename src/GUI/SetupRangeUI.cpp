#include "SetupRangeUI.hpp"
#include <BetterButtonSprite.hpp>
#include <AdvancedLabel/AdvLabelBMFont.hpp>
#include <Button.hpp>
#include <Utils.hpp>
#include <BetterAlertLayer.hpp>

using namespace geode::prelude;
using namespace qolmod;

SetupRangeUI* SetupRangeUI::create(Module* mod)
{
    auto pRet = new SetupRangeUI();

    pRet->module = mod;
    auto size = CCSizeMake(340 + 20, 250);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}


bool SetupRangeUI::setup()
{
    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);
    bg->setPosition(this->m_size / 2);
    this->setUserData(module);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChild(bg);

    auto title = AdvLabelBMFont::createWithLocalisation("edit-range-ui/title", "goldFont.fnt");
    title->setScale(0.7f);
    
    auto menu = CCMenu::create();
    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = Button::create(spr, this, menu_selector(PopupBase::onClose));
    menu->addChild(btn);

    auto trashSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    trashSpr->setScale(0.75f);

    auto trashBtn = Button::create(trashSpr, this, menu_selector(SetupRangeUI::onClear));
    auto trashMenu = CCMenu::create(trashBtn, nullptr);

    auto area = CCMenu::create();

    auto dash = CCLabelBMFont::create("-", "bigFont.fnt");
    dash->setPositionY(2.0f);
    dash->setScale(0.8f);
    min = BetterInputNode::create(60, "min");
    max = BetterInputNode::create(60, "max");
    min->setString("0.00");
    max->setString("0.00");
    min->setPositionX(-42);
    max->setPositionX(42);

    area->addChild(min);
    area->addChild(dash);
    area->addChild(max);

    auto add = Button::create(BetterButtonSprite::createWithLocalisation(ccp(67, 30), "edit-range-ui/add-button", "goldFont.fnt", "GJ_button_01.png"), this, menu_selector(SetupRangeUI::onAdd));

    menu->addChild(add);
    add->setPositionY(75);

    m_mainLayer->addChildAtPosition(area, Anchor::Center, ccp(50, 0));
    m_mainLayer->addChildAtPosition(createProgress(), Anchor::Top, ccp(0, -75));
    m_mainLayer->addChildAtPosition(trashMenu, Anchor::Top, ccp(135, -75));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(createInfo(), Anchor::TopRight, ccp(-16, -18));
    return true;
}

CCMenu* SetupRangeUI::createInfo()
{
    auto menu = CCMenu::create();

    auto infoBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(SetupRangeUI::onInfo));
    infoBtn->getNormalImage()->setScale(0.75f);

    menu->addChild(infoBtn, 67);

    if (!Mod::get()->getSavedValue<bool>("has-shown-range-info"))
    {
        pulsing = []{
            auto circle = PulsingCircle::create(2.5f, 18, 1.0f, false, false);
            circle->color = ccWHITE;
            return circle;
        }();
        menu->addChild(pulsing);
    }

    return menu;
}

CCSprite* SetupRangeUI::createProgress()
{
    auto progress = CCSprite::create("slidergroove2.png");

    auto progressBG = EasyBG::create();
    progressBG->setTargettingNode(progress);
    progressBG->setTargettingOffset(ccp(5, 5));
    progressBG->setPosition(progress->getContentSize() / 2);
    progress->addChild(progressBG, -3);

    std::function<CCSprite*()> createSpr = [progress]{
        auto progressInner = CCSprite::create("sliderBar2.png");
        progressInner->setAnchorPoint(ccp(0, 0.5f));

        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

        progressInner->getTexture()->setTexParameters(&params);
        progressInner->setContentWidth(progress->getContentWidth() - 2.0f * 2);
        progressInner->setTextureRect({0, 0, progressInner->getContentWidth(), progressInner->getContentHeight()});
        return progressInner;
    };

    auto white = createSpr();
    auto green = createSpr();
    auto red = createSpr();

    white->setPosition(ccp(2, 8));
    green->setColor(ccc3(0, 255, 0));
    red->setColor(ccc3(255, 0, 0));

    auto redStencil = CCNode::create();
    auto greenStencil = CCNode::create();
    redStencil->setScaleX(red->getContentWidth() / 100.0f);
    greenStencil->setScaleX(red->getContentWidth() / 100.0f);
    redClip = CCClippingNode::create(redStencil);
    greenClip = CCClippingNode::create(greenStencil);

    redClip->setPosition(ccp(2, 8));
    greenClip->setPosition(ccp(2, 8));
    redClip->addChild(red);
    greenClip->addChild(green);

    progress->addChild(white, -2);
    progress->addChild(greenClip, -2);
    progress->addChild(redClip, -2);

    updateProgress();
    return progress;
}

void SetupRangeUI::updateProgress()
{
    redClip->getStencil()->removeAllChildren();
    greenClip->getStencil()->removeAllChildren();

    for (auto& range : module->getRanges()->ranges)
    {
        auto n = CCLayerColor::create(ccc4(255, 255, 255, 255), range.max - range.min, 10);
        n->setAnchorPoint(ccp(0, 0.5f));
        n->ignoreAnchorPointForPosition(false);
        n->setPosition(ccp(range.min, 0));

        (range.enable ? greenClip->getStencil() : redClip->getStencil())->addChild(n);
    }
}

void SetupRangeUI::onInfo(CCObject* sender)
{
    Mod::get()->setSavedValue<bool>("has-shown-range-info", true);

    if (pulsing)
    {
        pulsing->removeFromParent();
        pulsing = nullptr;
    }

    BetterAlertLayer::createWithLocalisation("edit-range-ui/info-alert/title", "edit-range-ui/info-alert/text", "ui/ok-button")->show();
}

void SetupRangeUI::onClear(CCObject* sender)
{
    module->getRanges()->clear();
    updateProgress();
    module->genericSave();

    qolmod::utils::addCircleToNode(static_cast<CCNode*>(sender));
}

void SetupRangeUI::onAdd(CCObject* sender)
{
    module->getRanges()->addRange(qolmod::Range({
        .min = geode::utils::numFromString<double>(min->getString()).unwrapOr(0),
        .max = geode::utils::numFromString<double>(max->getString()).unwrapOr(0),
        .enable = true
    }));

    updateProgress();
    module->genericSave();
}