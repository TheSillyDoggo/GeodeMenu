#include "LanguageContributorsUI.hpp"
#include "../../GUI/BetterButtonSprite.hpp"
#include "../../GUI/EasyBG.hpp"
#include "../../Utils/ColourUtils.hpp"
#include "../LocalisationManager.hpp"
#include <BetterAlertLayer.hpp>

LanguageContributorsUI* LanguageContributorsUI::create(std::string lang)
{
    auto pRet = new LanguageContributorsUI();

    CCSize size = ccp(300, 230);
    pRet->lang = lang;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

SimplePlayer* LanguageContributorsUI::getPlayer(CLanguageContributor obj)
{
    auto player = SimplePlayer::create(obj.iconID);
    player->setColor(GameManager::get()->colorForIdx(obj.primaryCol));
    player->setSecondColor(GameManager::get()->colorForIdx(obj.secondaryCol));

    if (obj.glowEnabled)
    {
        player->setGlowOutline(GameManager::get()->colorForIdx(obj.glowCol));
        player->updateColors();
    }

    return player;
}

void LanguageContributorsUI::onPlayerProfile(CCObject* sender)
{
    if (!sender)
        return;

    if (!Mod::get()->setSavedValue<bool>("has-shown-language-credits-warning", true))
    {
        onInfo(sender);
        return;
    }

    auto pfp = ProfilePage::create(sender->getTag(), false);
    pfp->show();

    if (pfp->m_usernameLabel && std::string(pfp->m_usernameLabel->getString()) == "-")
    {
        pfp->m_usernameLabel->setString(static_cast<CCNode*>(sender)->getID().c_str());
    }
}

void LanguageContributorsUI::onInfo(CCObject* sender)
{
    auto alert = BetterAlertLayer::createWithLocalisation(this, "language-credits/warning/title", "language-credits/warning/text", "ui/ok-button", nullptr, 340, false, 0, 1.0f);
    alert->show();

    if (!static_cast<CCNode*>(sender)->getID().empty())
    {
        alert->setUserData(sender);
    }
}

void LanguageContributorsUI::FLAlert_Clicked(FLAlertLayer* layer, bool btn2)
{
    onPlayerProfile(static_cast<CCNode*>(layer->getUserData()));
}

bool LanguageContributorsUI::setup()
{
    language = LocalisationManager::get()->languageForPath(Mod::get()->getResourcesDir() / lang);    
    this->scheduleUpdate();

    m_bgSprite->setVisible(false);

    auto outline = CCScale9Sprite::create("GJ_square07.png");
    outline->setContentSize(m_size);
    outline->setAnchorPoint(ccp(0.5f, 0.5f));
    outline->setZOrder(69);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(outline, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("language-credits/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto titleBG = EasyBG::create();
    titleBG->setTargettingNode(title);
    titleBG->setTargettingOffset(ccp(4, 4));

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(LanguageContributorsUI::onClose));
    menu->addChild(btn);

    auto stencil = CCScale9Sprite::create("GJ_square01.png");
    stencil->setAnchorPoint(ccp(0.5f, 0.5f));
    stencil->setContentSize(m_size + ccp(-1, -1));

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.03f);

    auto node = CCNode::create();
    node->setContentSize(ccp(m_size.width, m_size.height));
    node->setAnchorPoint(ccp(0.5f, 0.5f));

    ground = GJGroundLayer::create(1, 0);
    ground->setAnchorPoint(ccp(0.5f, 0));
    ground->setScale(0.85f);
    ground->setPosition(ccp(m_size.width / 2, 69));
    ground->setContentWidth(getContentWidth());
    ground->ignoreAnchorPointForPosition(false);
    node->addChild(ground, 1);

    nodeBG = CCSprite::create(GameManager::get()->getBGTexture(1));
    nodeBG->setPosition(node->getContentSize() / 2);
    nodeBG->setScale(0.85f);
    node->addChild(nodeBG);

    clip->addChild(stencil);
    clip->addChild(node);

    int i = 0;
    auto contributors = language->getContributors();
    for (auto cont : contributors)
    {
        float spacing = 120;
        float x = ((m_size.width / 2) - (spacing * (contributors.size() - 1)) / 2) + spacing * i;

        auto icon = getPlayer(cont);
        icon->setZOrder(2);
        icon->setPosition(ccp(x, ground->getPositionY() + 30 / 2));

        node->addChild(icon);

        auto playerMenu = CCMenu::create();
        playerMenu->setPosition(icon->getPosition());

        auto name = CCLabelBMFont::create(cont.username.c_str(), "goldFont.fnt");
        name->setScale(0.7f);

        auto btn = CCMenuItemSpriteExtra::create(name, this, menu_selector(LanguageContributorsUI::onPlayerProfile));
        btn->setTag(cont.accountID);
        btn->setID(cont.username);
        btn->setPositionY(70);

        playerMenu->addChild(btn);

        node->addChild(playerMenu);
        i++;
    }

    auto infoMenu = CCMenu::create();

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.75f);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(LanguageContributorsUI::onInfo));
    infoMenu->addChild(infoBtn);

    m_mainLayer->addChildAtPosition(clip, Anchor::Center);
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(infoMenu, Anchor::TopRight, ccp(-15, -15));
    m_mainLayer->addChildAtPosition(titleBG, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    return true;
}

void LanguageContributorsUI::update(float dt)
{
    auto col = ColourUtils::get()->getPastel("language-credits-background");

    nodeBG->setColor(col);
    ground->updateGround01Color(ccc3(col.r * 0.7f, col.g * 0.7f, col.b * 0.7f));
}