#include "SetupRangeUI.hpp"
#include <BetterButtonSprite.hpp>
#include <AdvancedLabel/AdvLabelBMFont.hpp>
#include <Button.hpp>
#include <Utils.hpp>
#include <BetterAlertLayer.hpp>

using namespace geode::prelude;
using namespace qolmod;

constexpr static ccColor3B offColour = {150, 150, 150};

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
    auto trashMenu = CCMenu::create();
    trashMenu->addChild(trashBtn);

    auto area = CCMenu::create();

    auto dash = CCLabelBMFont::create("-", "bigFont.fnt");
    dash->setPositionY(2.0f);
    dash->setScale(0.8f);
    min = qolmod::InputNode::create(60, "min");
    max = qolmod::InputNode::create(60, "max");
    min->setString("0.00");
    max->setString("0.00");
    min->setPositionX(-42);
    max->setPositionX(42);

    toggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.8f);
    toggler->toggle(true);
    toggler->setPosition(ccp(0, -50));

    area->addChild(min);
    area->addChild(dash);
    area->addChild(max);
    area->addChild(toggler);

    auto add = Button::create(BetterButtonSprite::createWithLocalisation(ccp(67, 30), "edit-range-ui/add-button", "goldFont.fnt", "GJ_button_01.png"), this, menu_selector(SetupRangeUI::onAdd));

    menu->addChild(add);
    add->setPositionY(75);

    m_enabledToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SetupRangeUI::onEnabled), 0.65f);
    m_practiceToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SetupRangeUI::onPractice), 0.65f);
    m_notifyToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SetupRangeUI::onNotify), 0.65f);

    m_notifyToggler->setPosition(ccp(0, -24.5f) + ccp(-bg->getContentWidth() / 2, 0) + ccp(20, 20));
    m_practiceToggler->setPosition(m_notifyToggler->getPosition() + ccp(0, 27));
    m_enabledToggler->setPosition(m_practiceToggler->getPosition() + ccp(0, 27));

    m_enabledLabel = AdvLabelBMFont::createWithLocalisation("edit-range-ui/enabled-label", "goldFont.fnt");
    m_practiceLabel = AdvLabelBMFont::createWithLocalisation("edit-range-ui/practice-label", "goldFont.fnt");
    m_notifyLabel = AdvLabelBMFont::createWithLocalisation("edit-range-ui/notify-label", "goldFont.fnt");

    m_enabledLabel->setPosition(m_enabledToggler->getPosition() + ccp(16, 0));
    m_practiceLabel->setPosition(m_practiceToggler->getPosition() + ccp(16, 0));
    m_notifyLabel->setPosition(m_notifyToggler->getPosition() + ccp(16, 0));
    m_enabledLabel->setAnchorPoint(ccp(0, 0.5f));
    m_practiceLabel->setAnchorPoint(ccp(0, 0.5f));
    m_notifyLabel->setAnchorPoint(ccp(0, 0.5f));

    m_enabledLabel->limitLabelWidth(90, 0.55f, 0);
    m_practiceLabel->limitLabelWidth(90, 0.55f, 0);
    m_notifyLabel->limitLabelWidth(90, 0.55f, 0);

    menu->addChildren(m_enabledToggler, m_practiceToggler, m_notifyToggler, m_enabledLabel, m_practiceLabel, m_notifyLabel);

    m_perLevelError = AdvLabelBMFont::createWithLocalisation("edit-range-ui/per-level-error", "bigFont.fnt");
    m_perLevelError->setScale(0.4f);
    m_perLevelError->setMaxWidth(200 / 0.4f);
    m_perLevelError->setAlignment(kCCTextAlignmentCenter);

    m_mainLayer->addChildAtPosition(area, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(createProgress(), Anchor::Top, ccp(0, -75));
    m_mainLayer->addChildAtPosition(createModeMenu(), Anchor::Top, ccp(0, -45));
    m_mainLayer->addChildAtPosition(trashMenu, Anchor::Top, ccp(135, -75));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(m_perLevelError, Anchor::Top, ccp(0, -125));
    m_mainLayer->addChildAtPosition(createInfo(), Anchor::TopRight, ccp(-16, -18));
    updateConfig();
    return true;
}

CCMenu* SetupRangeUI::createModeMenu()
{
    auto menu = CCMenu::create();
    menu->setLayout(AxisLayout::create()->setGap(4.5f));
    menu->setScale(0.65f);

    m_modeBtns[ModRangeMode::Disabled] = qolmod::Button::create(BetterButtonSprite::create(ccp(106, 28), "Disabled", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(SetupRangeUI::onMode));
    m_modeBtns[ModRangeMode::Global] = qolmod::Button::create(BetterButtonSprite::create(ccp(106, 28), "Global", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(SetupRangeUI::onMode));
    m_modeBtns[ModRangeMode::PerLevel] = qolmod::Button::create(BetterButtonSprite::create(ccp(106, 28), "Per-Level", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(SetupRangeUI::onMode));

    m_modeBtns[ModRangeMode::Disabled]->m_scaleMultiplier = 1.1f;
    m_modeBtns[ModRangeMode::Global]->m_scaleMultiplier = 1.1f;
    m_modeBtns[ModRangeMode::PerLevel]->m_scaleMultiplier = 1.1f;

    menu->addChildren(m_modeBtns[ModRangeMode::Disabled], m_modeBtns[ModRangeMode::Global], m_modeBtns[ModRangeMode::PerLevel]);
    menu->updateLayout();
    return menu;
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
    m_outlineSprite = CCSprite::create("slidergroove2.png");

    auto progressBG = EasyBG::create();
    progressBG->setTargettingNode(m_outlineSprite);
    progressBG->setTargettingOffset(ccp(5, 5));
    progressBG->setPosition(m_outlineSprite->getContentSize() / 2);
    m_outlineSprite->addChild(progressBG, -3);

    std::function<CCSprite*()> createSpr = [&]{
        auto progressInner = CCSprite::create("sliderBar2.png");
        progressInner->setAnchorPoint(ccp(0, 0.5f));

        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

        progressInner->getTexture()->setTexParameters(&params);
        progressInner->setContentWidth(m_outlineSprite->getContentWidth() - 2.0f * 2);
        progressInner->setTextureRect({0, 0, progressInner->getContentWidth(), progressInner->getContentHeight()});
        return progressInner;
    };

    m_whiteSprite = createSpr();
    m_greenSprite = createSpr();
    m_redSprite = createSpr();

    m_whiteSprite->setPosition(ccp(2, 8));
    m_greenSprite->setColor(ccc3(0, 255, 0));
    m_redSprite->setColor(ccc3(255, 0, 0));

    auto redStencil = CCNode::create();
    auto greenStencil = CCNode::create();
    redStencil->setScaleX(m_redSprite->getContentWidth() / 100.0f);
    greenStencil->setScaleX(m_redSprite->getContentWidth() / 100.0f);
    redClip = CCClippingNode::create(redStencil);
    greenClip = CCClippingNode::create(greenStencil);

    redClip->setPosition(ccp(2, 8));
    greenClip->setPosition(ccp(2, 8));
    redClip->addChild(m_redSprite);
    greenClip->addChild(m_greenSprite);

    m_outlineSprite->addChild(m_whiteSprite, -2);
    m_outlineSprite->addChild(greenClip, -2);
    m_outlineSprite->addChild(redClip, -2);

    deleteMenu = CCMenu::create();
    deleteMenu->setPosition(m_whiteSprite->getPosition());
    deleteMenu->setScaleX(m_redSprite->getContentWidth() / 100.0f);
    deleteMenu->ignoreAnchorPointForPosition(false);
    deleteMenu->setAnchorPoint(ccp(0, 0.5f));
    deleteMenu->setContentSize(m_whiteSprite->getContentSize());

    m_outlineSprite->addChild(deleteMenu, 80085);
    return m_outlineSprite;
}

void SetupRangeUI::updateProgress()
{
    m_outlineSprite->setColor(m_config ? ccWHITE : offColour);
    m_whiteSprite->setColor(m_config ? ccWHITE : offColour);

    redClip->getStencil()->removeAllChildren();
    greenClip->getStencil()->removeAllChildren();
    deleteMenu->removeAllChildren();

    if (!m_config)
        return;

    int i = 0;
    for (auto& range : m_config->m_ranges.ranges)
    {
        auto n = CCLayerColor::create(ccc4(255, 255, 255, 255), range.max - range.min, 10);
        n->setAnchorPoint(ccp(0, 0.5f));
        n->ignoreAnchorPointForPosition(false);
        n->setPosition(ccp(range.min, 0));

        (range.enable ? greenClip->getStencil() : redClip->getStencil())->addChild(n);

        auto btn = Button::create(nullptr, this, menu_selector(SetupRangeUI::onRemove));
        btn->m_scaleMultiplier = 1.0f;
        btn->setAnchorPoint(ccp(0, 0.5f));
        btn->setContentSize(ccp(range.max - range.min, 24));
        btn->setPosition(ccp(range.min, deleteMenu->getContentHeight() / 2));
        btn->setTag(i++);
        deleteMenu->addChild(btn);
    }
}

void SetupRangeUI::onMode(CCObject* sender)
{
    for (auto& value : m_modeBtns)
    {
        if (value.second == sender)
        {
            RangeManager::get()->setMode(module, value.first);
            updateConfig();
            return;
        }
    }
}

void SetupRangeUI::onNotify(CCObject*)
{
    m_config->m_notify = !m_config->m_notify;
    RangeManager::get()->markSave();
}

void SetupRangeUI::onEnabled(CCObject*)
{
    m_config->m_enabled = !m_config->m_enabled;
    RangeManager::get()->markSave();
}

void SetupRangeUI::onPractice(CCObject*)
{
    m_config->m_practiceOnly = !m_config->m_practiceOnly;
    RangeManager::get()->markSave();
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
    if (!m_config)
        return;

    m_config->m_ranges.ranges.clear();
    updateProgress();
    RangeManager::get()->markSave();

    qolmod::utils::addCircleToNode(static_cast<CCNode*>(sender));
}

void SetupRangeUI::onAdd(CCObject* sender)
{
    m_config->m_ranges.addRange(qolmod::Range(
        geode::utils::numFromString<float>(min->getString()).unwrapOr(0),
        geode::utils::numFromString<float>(max->getString()).unwrapOr(0),
        toggler->isToggled()
    ));

    updateProgress();
    RangeManager::get()->markSave();
}

void SetupRangeUI::onRemove(CCObject* sender)
{
    m_config->m_ranges.ranges.erase(m_config->m_ranges.ranges.begin() + sender->getTag());
    updateProgress();
    RangeManager::get()->markSave();
}

void SetupRangeUI::updateButtons()
{
    auto mode = RangeManager::get()->getMode(module);
    for (auto& value : m_modeBtns)
    {
        value.second->setEnabled(value.first != mode);
        static_cast<BetterButtonSprite*>(value.second->getNormalImage())->setBGSprite(value.first == mode ? "GJ_button_02.png" : "GJ_button_05.png");
    }

    m_perLevelError->setVisible(!m_config && mode == ModRangeMode::PerLevel);

    m_enabledToggler->setEnabled(m_config);
    m_practiceToggler->setEnabled(m_config);
    m_notifyToggler->setEnabled(m_config);

    m_enabledToggler->m_offButton->setColor(m_config ? ccWHITE : offColour);
    m_practiceToggler->m_offButton->setColor(m_config ? ccWHITE : offColour);
    m_notifyToggler->m_offButton->setColor(m_config ? ccWHITE : offColour);

    m_enabledToggler->toggle(m_config && m_config->m_enabled);
    m_practiceToggler->toggle(m_config && m_config->m_practiceOnly);
    m_notifyToggler->toggle(m_config && m_config->m_notify);

    m_enabledLabel->setColor(m_config ? ccWHITE : offColour);
    m_practiceLabel->setColor(m_config ? ccWHITE : offColour);
    m_notifyLabel->setColor(m_config ? ccWHITE : offColour);
}

void SetupRangeUI::updateConfig()
{
    auto mode = RangeManager::get()->getMode(module);
    m_config = nullptr;

    if (mode == ModRangeMode::PerLevel)
    {
        auto lvl = utils::getCurrentLevel(false);
        
        if (lvl)
            m_config = RangeManager::get()->getConfig(RangeManager::get()->getKey(module, lvl));
    }
    else if (mode == ModRangeMode::Global)
    {
        m_config = RangeManager::get()->getConfig(RangeManager::get()->getKey(module, nullptr));
    }

    updateButtons();
    updateProgress();
}