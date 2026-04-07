#include "IconEffectsUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterInputNode.hpp"
#include <SetupColourConfigUI.hpp>
#include "IconicManager.hpp"
#include <BetterButtonSprite.hpp>
#include <BetterAlertLayer.hpp>
#include <ColourUtils.hpp>
#include <Utils.hpp>
#include <CCMenuExt.hpp>

using namespace qolmod;

bool IconEffectsUI::init()
{
    if (!CategoryNode::init())
        return false;

    this->scheduleUpdate();

    normalPage = CCNode::create();
    normalPage->setContentSize(getContentSize());

    profilesPage = CCNode::create();
    profilesPage->setContentSize(getContentSize());

    selectPage(isOnProfilesPage);

    bg->setVisible(false);
    auto bg = EasyBG::create();
    normalPage->addChildAtPosition(bg, Anchor::Top);
    bg->setAnchorPoint(ccp(0.5f, 1));
    bg->setContentWidth(getContentWidth());
    bg->setContentHeight(70);
    updateLayout();

    dualBG = EasyBG::create();
    dualBG->setAnchorPoint(ccp(0, 0));
    dualBG->setContentSize(ccp(110, getContentHeight() - bg->getContentHeight() - 2.5f * 2));
    dualBG->setZOrder(-69);

    effectsBG = EasyBG::create();
    effectsBG->setAnchorPoint(ccp(0, 0));
    effectsBG->setContentSize(ccp(getContentWidth() - dualBG->getContentWidth() - 2.5f * 2, getContentHeight() - bg->getContentHeight() - 2.5f * 2));
    effectsBG->setZOrder(-69);

    int x = 0;
    auto n = qolmod::CCMenuExt::create();

    for (size_t i = 1; i < (int)IconicGamemodeType::Swing + 1; i++)
    {
        auto p1 = createPreviewButton((IconicGamemodeType)i, false);
        auto p2 = createPreviewButton((IconicGamemodeType)i, true);

        p1->setPosition(ccp(x, 20));
        p2->setPosition(ccp(x, -20));

        x += p1->getContentWidth() + 15;

        n->addChild(p1);
        n->addChild(p2);
    }
    
    n->setContentSize(ccp(x - 25, 0));
    n->setPosition(ccp(2.5f + 30 / 2, getContentHeight() - 35));
    n->setAnchorPoint(ccp(0, 0));
    n->setScale(0.85f);

    auto menu = qolmod::CCMenuExt::create();
    menu->setContentSize(ccp(67, 30 * ((int)IconicEffectType::SpiderTeleport - 1)));
    menu->setPosition(ccp(15, 15));
    menu->setScale(0.85f);
    menu->setAnchorPoint(ccp(0, 0));
    menu->ignoreAnchorPointForPosition(true);

    scroll = geode::ScrollLayer::create(effectsBG->getContentSize() + ccp(-8, 0));
    scroll->setUserObject("MouseDispatcherExt"_spr, CCNode::create());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    scroll->setTouchEnabled(true);
    scroll->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());
    scroll->m_contentLayer->setContentHeight(menu->getScaledContentHeight() + 30);
    scroll->m_contentLayer->addChild(menu);
    scroll->moveToTop();
    menu->setLinkedRect(scroll);

    scrollbar = BetterScrollbar::create(scroll);
    scrollbar->setUseDynamicHandle(true);
    scrollbar->setAnchorPoint(ccp(1, 0));

    int y = 0;

    createEffectNodes(IconicEffectType::Primary, y++, menu);
    createEffectNodes(IconicEffectType::Secondary, y++, menu);
    createEffectNodes(IconicEffectType::Glow, y++, menu);
    createEffectNodes(IconicEffectType::Trail, y++, menu);
    createEffectNodes(IconicEffectType::Ghost, y++, menu);
    createEffectNodes(IconicEffectType::WaveTrail, y++, menu);
    createEffectNodes(IconicEffectType::DashFire, y++, menu);
    createEffectNodes(IconicEffectType::SpiderTeleport, y++, menu);
    createEffectNodes(IconicEffectType::FineOutline, y++, menu);

    updateSelection();

    auto dualMenu = qolmod::CCMenuExt::create();
    dualMenu->setAnchorPoint(ccp(0, 0));
    dualMenu->ignoreAnchorPointForPosition(false);
    dualMenu->setContentSize(dualBG->getContentSize());

    auto sprOff = BetterButtonSprite::createWithLocalisation(CCSizeMake(dualMenu->getContentWidth() - 15, 35), "iconic/seperate-label", "bigFont.fnt", "geode.loader/GE_button_05.png");
    sprOff->setMaxTextScale(0.4f);

    auto sprOn = BetterButtonSprite::createWithLocalisation(CCSizeMake(dualMenu->getContentWidth() - 15, 35), "iconic/seperate-label", "bigFont.fnt", "geode.loader/GE_button_01.png");
    sprOn->setMaxTextScale(0.4f);

    seperateToggle = CCMenuItemToggler::create(sprOff, sprOn, this, menu_selector(IconEffectsUI::onToggleSeperate));
    seperateToggle->toggle(IconicManager::get()->getSeperateColours());
    seperateToggle->setPosition(ccp(dualMenu->getContentWidth() / 2, 25.5f));
    seperateToggle->m_offButton->m_scaleMultiplier = 1.1f;
    seperateToggle->m_onButton->m_scaleMultiplier = 1.1f;

    dualMenu->addChild(seperateToggle);

    createDualNodes(IconicDualMode::Invert, 0, dualMenu);
    createDualNodes(IconicDualMode::Same, 1, dualMenu);
    createDualNodes(IconicDualMode::Seperate, 2, dualMenu);
    updateDualMode();

    auto profilesSpr = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
    profilesSpr->setScale(0.65f);

    auto profilesBtn = Button::create(profilesSpr, this, menu_selector(IconEffectsUI::onProfiles));
    profilesBtn->setPosition(ccp(dualMenu->getContentWidth() / 2, dualMenu->getContentHeight() - 30));
    dualMenu->addChild(profilesBtn);

    createProfilesPage();
    
    normalPage->addChild(n);
    normalPage->addChildAtPosition(dualBG, Anchor::BottomLeft);
    normalPage->addChildAtPosition(effectsBG, Anchor::BottomLeft, ccp(dualBG->getContentWidth() + 2.5f * 2, 0));
    normalPage->addChildAtPosition(dualMenu, Anchor::BottomLeft);
    normalPage->addChildAtPosition(scroll, Anchor::BottomLeft, ccp(115, 0));
    normalPage->addChildAtPosition(scrollbar, Anchor::BottomRight, ccp(0, 0));
    this->addChild(normalPage);
    this->addChild(profilesPage);
    return true;
}

void IconEffectsUI::createDualNodes(IconicDualMode mode, int y, CCMenu* menu)
{
    DualModeData data;
    data.mode = mode;

    data.toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(IconEffectsUI::onDualMode), 0.65f);
    data.toggler->setPosition(ccp(16, 118 - 27 * y));
    data.toggler->setTag((int)mode);

    std::string key;

    switch (mode)
    {
        case IconicDualMode::Invert:
            key = "iconic/dual-mode-invert";
            break;

        case IconicDualMode::Same:
            key = "iconic/dual-mode-same";
            break;
            
        case IconicDualMode::Seperate:
            key = "iconic/dual-mode-seperate";
            break;
    }

    data.label = AdvLabelBMFont::createWithLocalisation(key, "goldFont.fnt");
    data.label->setAnchorPoint(ccp(0, 0.5f));
    data.label->setPosition(ccp(32, data.toggler->getPositionY()));
    data.label->limitLabelWidth(70, 0.55f, 0);

    menu->addChild(data.toggler);
    menu->addChild(data.label);
    dualNodes.push_back(data);
}

void IconEffectsUI::updateDualMode()
{
    for (auto data : dualNodes)
    {
        bool enabled = IconicManager::get()->getDualMode() == data.mode;
        
        data.toggler->setEnabled(!enabled);
        data.toggler->toggle(enabled);
        data.label->setColor(enabled ? ccWHITE : ccc3(150, 150, 150));
    }
}

void IconEffectsUI::createEffectNodes(IconicEffectType type, int _y, CCMenu* menu)
{
    float y = menu->getContentHeight() + (_y * -30);
    EffectEditData data;

    data.toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(IconEffectsUI::onToggleOverride), 0.65f);
    data.toggler->setPosition(ccp(0, y));
    data.toggler->setTag((int)type);

    std::string formatStr = "";

    switch (type)
    {
        case IconicEffectType::Primary:
            formatStr = "iconic/override-primary-label";
            break;

        case IconicEffectType::Secondary:
            formatStr = "iconic/override-secondary-label";
            break;

        case IconicEffectType::Glow:
            formatStr = "iconic/override-glow-label";
            break;

        case IconicEffectType::Trail:
            formatStr = "iconic/override-trail-label";
            break;

        case IconicEffectType::Ghost:
            formatStr = "iconic/override-ghost-label";
            break;

        case IconicEffectType::WaveTrail:
            formatStr = "iconic/override-wave-trail-label";
            break;

        case IconicEffectType::DashFire:
            formatStr = "iconic/override-dash-fire-label";
            break;

        case IconicEffectType::SpiderTeleport:
            formatStr = "iconic/override-spider-teleport-label";
            break;

        case IconicEffectType::FineOutline:
            formatStr = "iconic/override-fine-outline-label";
            break;
    }

    data.label = AdvLabelBMFont::createWithLocalisation(formatStr, "bigFont.fnt");
    data.label->setPosition(ccp(16, y));
    data.label->setAnchorPoint(ccp(0, 0.5f));
    data.label->limitLabelWidth(170, 0.55f, 0);

    data.sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    data.sprite->setScale(0.65f);

    data.colourBtn = Button::create(data.sprite, this, menu_selector(IconEffectsUI::onEditColour));
    data.colourBtn->setTag((int)type);
    data.colourBtn->setPosition(ccp(220, y));
    // data.colourBtn->setPosition(ccp(data.label->getScaledContentWidth() + 35, y));

    menu->addChild(data.label);
    menu->addChild(data.colourBtn);
    menu->addChild(data.toggler);

    editNodes.emplace(type, data);
}

Button* IconEffectsUI::createPreviewButton(IconicGamemodeType gamemode, bool player2)
{
    PreviewNodeData data;
    data.gamemode = gamemode;
    data.player2 = player2;

    int id = 1;
    IconType type;
    auto gm = GameManager::get();

    switch (gamemode)
    {
        case IconicGamemodeType::Cube:
            id = gm->m_playerFrame;
            type = IconType::Cube;
            break;
        case IconicGamemodeType::Ship:
            id = gm->m_playerShip;
            type = IconType::Ship;
            break;
        case IconicGamemodeType::Jetpack:
            id = gm->m_playerJetpack;
            type = IconType::Jetpack;
            break;
        case IconicGamemodeType::Ball:
            id = gm->m_playerBall;
            type = IconType::Ball;
            break;
        case IconicGamemodeType::Ufo:
            id = gm->m_playerBird;
            type = IconType::Ufo;
            break;
        case IconicGamemodeType::Wave:
            id = gm->m_playerDart;
            type = IconType::Wave;
            break;
        case IconicGamemodeType::Robot:
            id = gm->m_playerRobot;
            type = IconType::Robot;
            break;
        case IconicGamemodeType::Spider:
            id = gm->m_playerSpider;
            type = IconType::Spider;
            break;
        case IconicGamemodeType::Swing:
            id = gm->m_playerSwing;
            type = IconType::Swing;
            break;
    }

    auto plr = SimplePlayer::create(id);
    plr->updatePlayerFrame(id, type);
    plr->setScale(0.9f);

    auto hk = IconicPlayerHook::create(plr);
    hk->setGamemode(gamemode, player2);
    plr->addChild(hk);

    auto n = CCNodeWithShader::create();
    n->addChild(plr);

    auto btn = Button::create(n, this, menu_selector(IconEffectsUI::onSelectType));
    btn->setTag((int)gamemode);
    btn->setUserObject("player2", player2 ? CCNode::create() : nullptr);
    btn->setContentSize(ccp(30, 30));

    if (gamemode == IconicGamemodeType::Dart)
        btn->setContentSize(ccp(27.5f, 27.5f));

    n->setPosition(btn->getContentSize() / 2);

    data.hook = hk;
    data.shader = n;
    data.simple = plr;
    data.btn = btn;
    previewNodes.push_back(data);
    return btn;
}

std::pair<IconicGamemodeType, bool> IconEffectsUI::getSelected()
{
    IconicGamemodeType gamemode = selectedType.first;
    bool player2 = selectedType.second;

    if (IconicManager::get()->getDualMode() != IconicDualMode::Seperate)
        player2 = false;

    if (!IconicManager::get()->getSeperateColours())
        gamemode = IconicGamemodeType::Cube;

    return { gamemode, player2 };
}

void IconEffectsUI::updateSelection()
{
    for (auto data : previewNodes)
    {
        bool chk1 = selectedType.first == data.gamemode;
        bool chk2 = selectedType.second == data.player2;

        if (IconicManager::get()->getDualMode() != IconicDualMode::Seperate)
            chk2 = true;

        if (!IconicManager::get()->getSeperateColours())
            chk1 = true;

        bool sel = chk1 && chk2;

        data.btn->setEnabled(!sel);
        data.shader->setColor(sel ? ccc3(255, 255, 255) : ccc3(100, 100, 100));
    }

    auto config = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    editNodes[IconicEffectType::Primary].toggler->toggle(config->getUseOverride(IconicEffectType::Primary));
    editNodes[IconicEffectType::Secondary].toggler->toggle(config->getUseOverride(IconicEffectType::Secondary));
    editNodes[IconicEffectType::Glow].toggler->toggle(config->getUseOverride(IconicEffectType::Glow));
    editNodes[IconicEffectType::Trail].toggler->toggle(config->getUseOverride(IconicEffectType::Trail));
    editNodes[IconicEffectType::Ghost].toggler->toggle(config->getUseOverride(IconicEffectType::Ghost));
    editNodes[IconicEffectType::WaveTrail].toggler->toggle(config->getUseOverride(IconicEffectType::WaveTrail));
    editNodes[IconicEffectType::DashFire].toggler->toggle(config->getUseOverride(IconicEffectType::DashFire));
    editNodes[IconicEffectType::SpiderTeleport].toggler->toggle(config->getUseOverride(IconicEffectType::SpiderTeleport));
    editNodes[IconicEffectType::FineOutline].toggler->toggle(config->getUseOverride(IconicEffectType::FineOutline));

    updateOverride();
}

void IconEffectsUI::updateOverride()
{
    auto config = IconicManager::get()->getConfig(getSelected().first, getSelected().second);
    ccColor3B disabled = ccc3(150, 150, 150);

    editNodes[IconicEffectType::Primary].label->setColor(config->getUseOverride(IconicEffectType::Primary) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Secondary].label->setColor(config->getUseOverride(IconicEffectType::Secondary) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Glow].label->setColor(config->getUseOverride(IconicEffectType::Glow) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Trail].label->setColor(config->getUseOverride(IconicEffectType::Trail) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Ghost].label->setColor(config->getUseOverride(IconicEffectType::Ghost) ? ccWHITE : disabled);
    editNodes[IconicEffectType::WaveTrail].label->setColor(config->getUseOverride(IconicEffectType::WaveTrail) ? ccWHITE : disabled);
    editNodes[IconicEffectType::DashFire].label->setColor(config->getUseOverride(IconicEffectType::DashFire) ? ccWHITE : disabled);
    editNodes[IconicEffectType::SpiderTeleport].label->setColor(config->getUseOverride(IconicEffectType::SpiderTeleport) ? ccWHITE : disabled);
    editNodes[IconicEffectType::FineOutline].label->setColor(config->getUseOverride(IconicEffectType::FineOutline) ? ccWHITE : disabled);

    if (!IconicManager::get()->isFineOutlineLoaded())
    {
        editNodes[IconicEffectType::FineOutline].toggler->m_offButton->setColor(ccc3(150, 150, 150));
    }
}

void IconEffectsUI::selectPage(bool profiles)
{
    isOnProfilesPage = profiles;
    normalPage->setVisible(!profiles);
    profilesPage->setVisible(profiles);
}

void IconEffectsUI::onProfiles(CCObject* sender)
{
    selectPage(!isOnProfilesPage);
}

void IconEffectsUI::onSelectType(CCObject* sender)
{
    auto n = static_cast<CCNode*>(sender);
    selectedType.first = (IconicGamemodeType)n->getTag();
    selectedType.second = n->getUserObject("player2");

    qolmod::utils::addCircleToNode(n);

    updateSelection();
}

void IconEffectsUI::onToggleOverride(CCObject* sender)
{
    if ((IconicEffectType)sender->getTag() == IconicEffectType::FineOutline)
    {
        if (!IconicManager::get()->isFineOutlineLoaded())
        {
            auto toggler = editNodes[IconicEffectType::FineOutline].toggler;
            toggler->toggle(!false);
            toggler->m_offButton->setScale(1.0f);

            BetterAlertLayer::createWithLocalisation("ui/title-error", "iconic/fine-outline-missing-error", "ui/ok-button")->show();
            return;
        }
    }

    auto config = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    qolmod::utils::addCircleToNode(static_cast<CCNode*>(sender), 0.65f);

    config->setUseOverride((IconicEffectType)sender->getTag(), !config->getUseOverride((IconicEffectType)sender->getTag()));

    updateOverride();
}

void IconEffectsUI::onToggleSeperate(CCObject* sender)
{
    IconicManager::get()->setSeperateColours(!IconicManager::get()->getSeperateColours());

    updateSelection();
}

void IconEffectsUI::onDualMode(CCObject* sender)
{
    IconicManager::get()->setDualMode((IconicDualMode)sender->getTag());

    qolmod::utils::addCircleToNode(static_cast<CCNode*>(sender), 0.65f);

    updateSelection();
    updateDualMode();
}

void IconEffectsUI::onEditColour(CCObject* sender)
{
    auto gm = getSelected().first;
    auto type = (IconicEffectType)sender->getTag();

    if (type == IconicEffectType::WaveTrail)
        gm = IconicGamemodeType::Dart;

    auto conf = IconicManager::get()->getConfig(gm, getSelected().second);

    auto ui = SetupColourConfigUI::create([this, conf, type](ColourConfig con)
    {
        switch (type)
        {
            case IconicEffectType::Primary:
                conf->setPrimaryConfig(con);
                break;

            case IconicEffectType::Secondary:
                conf->setSecondaryConfig(con);
                break;

            case IconicEffectType::Glow:
                conf->setGlowConfig(con);
                break;

            case IconicEffectType::Trail:
                conf->setTrailConfig(con);
                break;

            case IconicEffectType::Ghost:
                conf->setGhostConfig(con);
                break;

            case IconicEffectType::WaveTrail:
                conf->setWaveTrailConfig(con);
                break;

            case IconicEffectType::DashFire:
                conf->setDashFireConfig(con);
                break;

            case IconicEffectType::SpiderTeleport:
                conf->setSpiderTeleportConfig(con);
                break;

            case IconicEffectType::FineOutline:
                conf->setFineOutlineConfig(con);
                break;
        }
    });

    switch (type)
    {
        case IconicEffectType::Primary:
            ui->setStartConfig(conf->getPrimaryConfig());
            break;

        case IconicEffectType::Secondary:
            ui->setStartConfig(conf->getSecondaryConfig());
            break;

        case IconicEffectType::Glow:
            ui->setStartConfig(conf->getGlowConfig());
            break;

        case IconicEffectType::Trail:
            ui->setStartConfig(conf->getTrailConfig());
            break;

        case IconicEffectType::Ghost:
            ui->setStartConfig(conf->getGhostConfig());
            break;

        case IconicEffectType::WaveTrail:
            ui->setStartConfig(conf->getWaveTrailConfig());
            break;

        case IconicEffectType::DashFire:
            ui->setStartConfig(conf->getDashFireConfig());
            break;

        case IconicEffectType::SpiderTeleport:
            ui->setStartConfig(conf->getSpiderTeleportConfig());
            break;

        case IconicEffectType::FineOutline:
            ui->setStartConfig(conf->getFineOutlineConfig());
            break;
    }

    ui->show();
}

#define DISABLED_CHECK($type) \
if (!config->getUseOverride(IconicEffectType::$type)) \
    editNodes[IconicEffectType::$type].sprite->setColor(ccc3( \
        editNodes[IconicEffectType::$type].sprite->getColor().r * (100.0f / 255.0f), \
        editNodes[IconicEffectType::$type].sprite->getColor().g * (100.0f / 255.0f), \
        editNodes[IconicEffectType::$type].sprite->getColor().b * (100.0f / 255.0f) \
    )); \
editNodes[IconicEffectType::$type].colourBtn->setEnabled(config->getUseOverride(IconicEffectType::$type));

void IconEffectsUI::update(float dt)
{
    auto config = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    editNodes[IconicEffectType::Primary].sprite->setColor(config->getPrimary());
    editNodes[IconicEffectType::Secondary].sprite->setColor(config->getSecondary());
    editNodes[IconicEffectType::Glow].sprite->setColor(config->getGlow());
    editNodes[IconicEffectType::Trail].sprite->setColor(config->getTrail());
    editNodes[IconicEffectType::Ghost].sprite->setColor(config->getGhost());
    editNodes[IconicEffectType::WaveTrail].sprite->setColor(config->getWaveTrail());
    editNodes[IconicEffectType::DashFire].sprite->setColor(config->getDashFire());
    editNodes[IconicEffectType::SpiderTeleport].sprite->setColor(config->getSpiderTeleport());
    editNodes[IconicEffectType::FineOutline].sprite->setColor(config->getFineOutline());

    DISABLED_CHECK(Primary);
    DISABLED_CHECK(Secondary);
    DISABLED_CHECK(Glow);
    DISABLED_CHECK(Trail);
    DISABLED_CHECK(Ghost);
    DISABLED_CHECK(WaveTrail);
    DISABLED_CHECK(DashFire);
    DISABLED_CHECK(SpiderTeleport);
    DISABLED_CHECK(FineOutline);
}

void IconEffectsUI::updateFineOutline()
{

}

void IconEffectsUI::createProfilesPage()
{
    auto closeMenu = CCMenu::create();
    auto closeBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), this, menu_selector(IconEffectsUI::onProfiles));
    closeMenu->addChild(closeBtn);

    profilesPage->addChildAtPosition(closeMenu, Anchor::TopLeft, ccp(30, -30));
}