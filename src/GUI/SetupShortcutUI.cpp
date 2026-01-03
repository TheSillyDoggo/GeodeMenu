#include "SetupShortcutUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "BlurLayer.hpp"
#include "BetterButtonSprite.hpp"
#include "EasyBG.hpp"
#include "SelectItemUI.hpp"

SetupShortcutUI* SetupShortcutUI::create(std::function<void(bool, ModuleShortcutConfig)> onFinish)
{
    auto pRet = new SetupShortcutUI();

    CCSize size = ccp(300, 240);
    pRet->onFinish = onFinish;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void SetupShortcutUI::setStartConfig(bool enabled, ModuleShortcutConfig conf)
{
    this->shortcutEnabled = enabled;
    this->conf = conf;

    updateUI();
}

void SetupShortcutUI::updateUI()
{
    enabledToggle->toggle(shortcutEnabled);
    movableToggle->toggle(conf.isMovable);
    scaleInput->setString(utils::numToString<float>(conf.scale, 2));
    opacityInput->setString(utils::numToString<float>(conf.opacity, 2));

    visInMenu->toggle(conf.visibility.showInMenu);
    visInGame->toggle(conf.visibility.showInGame);
    visInGamePause->toggle(conf.visibility.showInPauseMenu);
    visInEditor->toggle(conf.visibility.showInEditor);
    visInEditorPause->toggle(conf.visibility.showInEditorPauseMenu);

    bgOffSpr->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(conf.bgOffSprite.empty() ? "GJ_deleteIcon_001.png" : conf.bgOffSprite.c_str()));
    bgOnSpr->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(conf.bgOffSprite.empty() ? "GJ_deleteIcon_001.png" : conf.bgOnSprite.c_str()));

    if (conf.shortcutOverlay.empty())
        overlaySpr->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("GJ_deleteIcon_001.png"));
    else
    {
        auto tex = CCTextureCache::get()->addImage(conf.shortcutOverlay.c_str(), false);

        overlaySpr->setTexture(tex);
        overlaySpr->setTextureRect(CCRectMake(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    }

    bgOffSpr->setScale((25 - 10) / std::max<float>(bgOffSpr->getContentHeight(), bgOffSpr->getContentWidth()));
    bgOnSpr->setScale((25 - 10) / std::max<float>(bgOnSpr->getContentHeight(), bgOnSpr->getContentWidth()));
    overlaySpr->setScale((25 - 10) / std::max<float>(overlaySpr->getContentHeight(), overlaySpr->getContentWidth()));
}

void SetupShortcutUI::onClose(CCObject* sender)
{
    shortcutEnabled = enabledToggle->isToggled();
    conf.isMovable = movableToggle->isToggled();
    conf.visibility.showInMenu = visInMenu->isToggled();
    conf.visibility.showInGame = visInGame->isToggled();
    conf.visibility.showInPauseMenu = visInGamePause->isToggled();
    conf.visibility.showInEditor = visInEditor->isToggled();
    conf.visibility.showInEditorPauseMenu = visInEditorPause->isToggled();

    conf.scale = utils::numFromString<float>(scaleInput->getString()).unwrapOr(conf.scale);
    conf.opacity = utils::numFromString<float>(opacityInput->getString()).unwrapOr(conf.opacity);

    if (onFinish)
        onFinish(shortcutEnabled, conf);

    geode::Popup<>::onClose(sender);
}

void SetupShortcutUI::onSelectSprite(CCObject* sender)
{
    SelectItemUI* ui = nullptr;

    switch (sender->getTag())
    {
        case -1:
            ui = SelectItemUI::create(SelectItemType::ShortcutBG, [this](std::string spr)
            {
                conf.bgOffSprite = spr;
                updateUI();
            });
            ui->setSelectedItem(conf.bgOffSprite);
            ui->show();

            break;

        case 0:
            ui = SelectItemUI::create(SelectItemType::ShortcutBG, [this](std::string spr)
            {
                conf.bgOnSprite = spr;
                updateUI();
            });
            ui->setSelectedItem(conf.bgOnSprite);
            ui->show();

            break;

        case 1:
            ui = SelectItemUI::create(SelectItemType::ShortcutOverlay, [this](std::string spr)
            {
                conf.shortcutOverlay = spr;
                updateUI();
            });
            ui->setSelectedItem(conf.shortcutOverlay);
            ui->show();

            break;
    }
}

bool SetupShortcutUI::setup()
{
    this->addChild(CCBlurLayer::create(), -3);

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("setup-shortcut/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();
    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SetupShortcutUI::onClose));
    menu->addChild(btn);

    float size = 25;
    float spacing = 2.5f;

    auto enabledMenu = CCMenu::create();
    auto spritesMenu = CCMenu::create();
    auto visMenu = CCMenu::create();
    visMenu->setAnchorPoint(ccp(0, 0));
    visMenu->setScale(0.85f);

    for (int i = -1; i < 2; i++)
    {
        auto node = CCMenuItemSpriteExtra::create(CCNode::create(), nullptr, this, menu_selector(SetupShortcutUI::onSelectSprite));
        node->setContentSize(ccp(size, size));
        node->setAnchorPoint(ccp(0.5f, 0.5f));
        node->setPositionX((size + spacing) * i);
        node->setTag(i);

        auto sprAround = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
        sprAround->setPosition(node->getContentSize() / 2);
        sprAround->setScale(size / sprAround->getContentHeight());

        auto n = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
        n->setPosition(node->getContentSize() / 2);
        n->setAnchorPoint(ccp(0.5f, 0.5f));
        
        if (i == -1)
            bgOffSpr = n;
        else if (i == 0)
            bgOnSpr = n;
        else
            overlaySpr = n;

        node->addChild(sprAround);
        node->addChild(n);

        spritesMenu->addChild(node);
    }
    

    auto spritesBG = EasyBG::create();
    spritesBG->setContentSize(ccp(size * 3 + spacing * (3 + 1), size + spacing * 2));

    auto visibleBG = EasyBG::create();
    visibleBG->setAnchorPoint(ccp(0, 0.5f));
    visibleBG->setContentSize(ccp(115, 140));

    auto scaleTitle = AdvLabelBMFont::createWithLocalisation("setup-shortcut/scale-title", "bigFont.fnt");
    auto opacityTitle = AdvLabelBMFont::createWithLocalisation("setup-shortcut/opacity-title", "bigFont.fnt");
    scaleTitle->limitLabelWidth(65, 0.45f, 0);
    opacityTitle->limitLabelWidth(65, 0.45f, 0);

    scaleInput = BetterInputNode::create(70, "Scale");
    scaleInput->setCharFilter("1234567890.");
    scaleInput->setNumHoldValues(true, 0.05f, 5, 0.8f);
    scaleInput->setAnchorPoint(ccp(1, 0.5f));
    opacityInput = BetterInputNode::create(70, "Opacity");
    opacityInput->setCharFilter("1234567890.");
    opacityInput->setNumHoldValues(true, 0.05f, 5, 0.8f);
    opacityInput->setAnchorPoint(ccp(1, 0.5f));

    enabledToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);

    movableToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    movableToggle->setPositionY(-30);

    auto lblEnabled = AdvLabelBMFont::createWithLocalisation("setup-shortcut/enabled-title", "bigFont.fnt");
    auto movEnabled = AdvLabelBMFont::createWithLocalisation("setup-shortcut/movable-title", "bigFont.fnt");

    lblEnabled->setPosition(enabledToggle->getPosition() + ccp(18, 0));
    lblEnabled->setAnchorPoint(ccp(0, 0.5f));
    lblEnabled->limitLabelWidth(80, 0.5f, 0);

    movEnabled->setPosition(movableToggle->getPosition() + ccp(18, 0));
    movEnabled->setAnchorPoint(ccp(0, 0.5f));
    movEnabled->limitLabelWidth(80, 0.5f, 0);

    enabledMenu->addChild(enabledToggle);
    enabledMenu->addChild(movableToggle);
    enabledMenu->addChild(lblEnabled);
    enabledMenu->addChild(movEnabled);

    visInMenu = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    visInGame = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    visInGamePause = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    visInEditor = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    visInEditorPause = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);

    visInMenu       ->setPositionY(0 * -30);
    visInGame       ->setPositionY(1 * -30);
    visInGamePause  ->setPositionY(2 * -30);
    visInEditor     ->setPositionY(3 * -30);
    visInEditorPause->setPositionY(4 * -30);

    auto lblInMenu = AdvLabelBMFont::createWithLocalisation("setup-shortcut/in-menu", "bigFont.fnt");
    auto lblInGame = AdvLabelBMFont::createWithLocalisation("setup-shortcut/in-game", "bigFont.fnt");
    auto lblInGamePause = AdvLabelBMFont::createWithLocalisation("setup-shortcut/in-game-pause", "bigFont.fnt");
    auto lblInEditor = AdvLabelBMFont::createWithLocalisation("setup-shortcut/in-editor", "bigFont.fnt");
    auto lblInEditorPause = AdvLabelBMFont::createWithLocalisation("setup-shortcut/in-editor-pause", "bigFont.fnt");

    lblInMenu->setPosition(visInMenu->getPosition() + ccp(18, 0));
    lblInMenu->setAnchorPoint(ccp(0, 0.5f));
    lblInMenu->limitLabelWidth(80, 0.5f, 0);

    lblInGame->setPosition(visInGame->getPosition() + ccp(18, 0));
    lblInGame->setAnchorPoint(ccp(0, 0.5f));
    lblInGame->limitLabelWidth(80, 0.5f, 0);

    lblInGamePause->setPosition(visInGamePause->getPosition() + ccp(18, 0));
    lblInGamePause->setAnchorPoint(ccp(0, 0.5f));
    lblInGamePause->limitLabelWidth(80, 0.5f, 0);

    lblInEditor->setPosition(visInEditor->getPosition() + ccp(18, 0));
    lblInEditor->setAnchorPoint(ccp(0, 0.5f));
    lblInEditor->limitLabelWidth(80, 0.5f, 0);

    lblInEditorPause->setPosition(visInEditorPause->getPosition() + ccp(18, 0));
    lblInEditorPause->setAnchorPoint(ccp(0, 0.5f));
    lblInEditorPause->limitLabelWidth(80, 0.5f, 0);

    visMenu->addChild(visInMenu);
    visMenu->addChild(visInGame);
    visMenu->addChild(visInGamePause);
    visMenu->addChild(visInEditor);
    visMenu->addChild(visInEditorPause);
    visMenu->addChild(lblInMenu);
    visMenu->addChild(lblInGame);
    visMenu->addChild(lblInGamePause);
    visMenu->addChild(lblInEditor);
    visMenu->addChild(lblInEditorPause);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(enabledMenu, Anchor::Center, ccp(0, 60));
    m_mainLayer->addChildAtPosition(spritesBG, Anchor::Right, ccp(-117, -5));
    m_mainLayer->addChildAtPosition(spritesMenu, Anchor::Right, ccp(-117, -5));
    m_mainLayer->addChildAtPosition(visibleBG, Anchor::Left, ccp(15, -5));
    m_mainLayer->addChildAtPosition(visMenu, Anchor::Left, ccp(35, 45));
    m_mainLayer->addChildAtPosition(scaleInput, Anchor::Right, ccp(-15 - 5 - 70, -60));
    m_mainLayer->addChildAtPosition(opacityInput, Anchor::Right, ccp(-15, -60));
    m_mainLayer->addChildAtPosition(scaleTitle, Anchor::Right, ccp(-15 - 5 - 70, -60) + ccp(-70 / 2, 27));
    m_mainLayer->addChildAtPosition(opacityTitle, Anchor::Right, ccp(-15, -60) + ccp(-70 / 2, 27));
    return true;
}