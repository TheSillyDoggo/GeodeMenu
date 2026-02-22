#include "IconEffectsUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterInputNode.hpp"
#include <SetupColourConfigUI.hpp>
#include "IconicManager.hpp"
#include <BetterButtonSprite.hpp>
#include <ColourUtils.hpp>

bool IconEffectsUI::init()
{
    if (!CategoryNode::init())
        return false;

    this->scheduleUpdate();

    bg->setAnchorPoint(ccp(0.5f, 1));
    static_cast<AnchorLayoutOptions*>(bg->getLayoutOptions())->setAnchor(Anchor::Top);
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
    auto n = CCMenu::create();

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

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(0, 0));

    for (size_t i = 1; i < (int)IconicEffectType::WaveTrail + 1; i++)
    {
        createEffectNodes((IconicEffectType)i, i - 1, menu);
    }

    updateSelection();

    auto dualMenu = CCMenu::create();
    dualMenu->setAnchorPoint(ccp(0, 0));
    dualMenu->ignoreAnchorPointForPosition(false);
    dualMenu->setContentSize(dualBG->getContentSize());

    auto sprOff = BetterButtonSprite::createWithLocalisation(CCSizeMake(dualMenu->getContentWidth() - 15, 35), "iconic/seperate-label", "bigFont.fnt", "geode.loader/GE_button_05.png");
    sprOff->setMaxTextScale(0.4f);

    auto sprOn = BetterButtonSprite::createWithLocalisation(CCSizeMake(dualMenu->getContentWidth() - 15, 35), "iconic/seperate-label", "bigFont.fnt", "geode.loader/GE_button_01.png");
    sprOn->setMaxTextScale(0.4f);

    seperateToggle = CCMenuItemToggler::create(sprOn, sprOff, this, menu_selector(IconEffectsUI::onToggleSeperate));
    seperateToggle->toggle(IconicManager::get()->getSeperateColours());
    seperateToggle->setPosition(ccp(dualMenu->getContentWidth() / 2, 25.5f));
    seperateToggle->m_offButton->m_scaleMultiplier = 1.1f;
    seperateToggle->m_onButton->m_scaleMultiplier = 1.1f;

    dualMenu->addChild(seperateToggle);

    createDualNodes(IconicDualMode::Invert, 0, dualMenu);
    createDualNodes(IconicDualMode::Same, 1, dualMenu);
    createDualNodes(IconicDualMode::Seperate, 2, dualMenu);
    
    this->addChild(n);
    this->addChildAtPosition(dualBG, Anchor::BottomLeft);
    this->addChildAtPosition(effectsBG, Anchor::BottomLeft, ccp(dualBG->getContentWidth() + 2.5f * 2, 0));
    this->addChildAtPosition(menu, Anchor::BottomLeft, ccp(dualBG->getContentWidth() + 2.5f * 2, effectsBG->getContentHeight()) + ccp(17.5f, -17.5f));
    this->addChildAtPosition(dualMenu, Anchor::BottomLeft);
    return true;
}

void IconEffectsUI::createDualNodes(IconicDualMode mode, int y, CCMenu* menu)
{
    DualModeData data;
    data.mode = mode;

    data.toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(IconEffectsUI::onDualMode), 0.65f);
    data.toggler->setPosition(ccp(20, 100 - 20 * y));
    data.toggler->setTag((int)mode);

    menu->addChild(data.toggler);
    dualNodes.push_back(data);
}

void IconEffectsUI::createEffectNodes(IconicEffectType type, int _y, CCMenu* menu)
{
    float y = _y * -30;
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
    }

    data.label = AdvLabelBMFont::createWithLocalisation(formatStr, "bigFont.fnt");
    data.label->setPosition(ccp(16, y));
    data.label->setAnchorPoint(ccp(0, 0.5f));
    data.label->limitLabelWidth(140, 0.55f, 0);

    data.sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    data.sprite->setScale(0.65f);

    data.colourBtn = CCMenuItemSpriteExtra::create(data.sprite, this, menu_selector(IconEffectsUI::onEditColour));
    data.colourBtn->setTag((int)type);
    data.colourBtn->setPosition(ccp(190, y));
    // data.colourBtn->setPosition(ccp(data.label->getScaledContentWidth() + 35, y));

    menu->addChild(data.label);
    menu->addChild(data.colourBtn);
    menu->addChild(data.toggler);

    editNodes.emplace(type, data);
}

CCMenuItemSpriteExtra* IconEffectsUI::createPreviewButton(IconicGamemodeType gamemode, bool player2)
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

    auto btn = CCMenuItemSpriteExtra::create(n, this, menu_selector(IconEffectsUI::onSelectType));
    btn->setTag((int)gamemode);
    btn->setUserFlag("player2", player2);
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
}

void IconEffectsUI::onSelectType(CCObject* sender)
{
    auto n = static_cast<CCNode*>(sender);
    selectedType.first = (IconicGamemodeType)n->getTag();
    selectedType.second = n->getUserFlag("player2");

    auto circle = CCCircleWave::create(0, 25, 0.2f, true);
    circle->setPosition(n->getContentSize() / 2);
    circle->m_circleMode = CircleMode::Outline;
    circle->setUserFlag("allow-circle"_spr, true);

    n->addChild(circle);

    updateSelection();
}

void IconEffectsUI::onToggleOverride(CCObject* sender)
{
    auto config = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    config->setUseOverride((IconicEffectType)sender->getTag(), !config->getUseOverride((IconicEffectType)sender->getTag()));
}


void IconEffectsUI::onToggleSeperate(CCObject* sender)
{
    IconicManager::get()->setSeperateColours(!IconicManager::get()->getSeperateColours());

    updateSelection();
}

void IconEffectsUI::onDualMode(CCObject* sender)
{
    IconicManager::get()->setDualMode((IconicDualMode)sender->getTag());

    updateSelection();
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

    ccColor3B disabled = ccc3(150, 150, 150);

    editNodes[IconicEffectType::Primary].label->setColor(config->getUseOverride(IconicEffectType::Primary) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Secondary].label->setColor(config->getUseOverride(IconicEffectType::Secondary) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Glow].label->setColor(config->getUseOverride(IconicEffectType::Glow) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Trail].label->setColor(config->getUseOverride(IconicEffectType::Trail) ? ccWHITE : disabled);
    editNodes[IconicEffectType::Ghost].label->setColor(config->getUseOverride(IconicEffectType::Ghost) ? ccWHITE : disabled);
    editNodes[IconicEffectType::WaveTrail].label->setColor(config->getUseOverride(IconicEffectType::WaveTrail) ? ccWHITE : disabled);

    DISABLED_CHECK(Primary);
    DISABLED_CHECK(Secondary);
    DISABLED_CHECK(Glow);
    DISABLED_CHECK(Trail);
    DISABLED_CHECK(Ghost);
    DISABLED_CHECK(WaveTrail);
}