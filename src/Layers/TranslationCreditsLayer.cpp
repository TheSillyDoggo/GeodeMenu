#include "TranslationCreditsLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include "../UI/PlayerDeathAnimation.hpp"
#include "../Utils/TranslationManager.hpp"

// Particle Strings

#define PARTICLE_HIT_BIG "30a-1a1a0a-1a90a0a46a76a11a0a0a-483a45a0a0a0a1a1a0a0a1a0a1a0a1a0a1a0a0a1a0a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0"
#define PARTICLE_HIT_MEDIUM "30a-1a1a0a30a90a0a5a0a11a0a0a-208a289a0a0a0a1a1a0a0a1a0a1a0a1a0a1a0a0a1a0a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0"
#define PARTICLE_HIT_SMALL "30a-1a1a0a30a90a70a58a0a11a0a0a-208a0a0a0a0a1a1a0a0a1a0a1a0a1a0a1a0a0a1a0a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0"

void TranslationCreditsLayer::customSetup()
{
    auto langNode = CCNode::create();
    langNode->setContentWidth(6969);
    langNode->setAnchorPoint(ccp(0.5f, 0.5f));
    langNode->setScale(0.5f);
    
    auto langNative = CCLabelTTF::create(language["display_name_native"].asString().unwrapOr("Missing field: display_name_native").c_str(), "Arial", 32.5f);
    auto langEng = CCLabelBMFont::create(fmt::format("({})", language["display_name_english"].asString().unwrapOr("Missing field: display_name_english")).c_str(), "bigFont.fnt");

    langNode->addChild(langNative);
    langNode->addChild(langEng);

    langNode->setLayout(AxisLayout::create()->setGap(20));

    auto stencil = CCScale9Sprite::create("banner-mask.png"_spr);
    stencil->setContentSize(size + ccp(0, -32));
    stencil->setAnchorPoint(ccp(0.5f, 1));
    stencil->setScaleY(-1);

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.03f);
    clip->setAnchorPoint(ccp(0.5f, 0));
    clip->setZOrder(3);

    auto clipOutline = CCScale9Sprite::create("GJ_square07.png");
    clipOutline->setContentSize(size);
    clipOutline->setAnchorPoint(ccp(0.5f, 0));
    clip->addChild(clipOutline, 420);

    ground = GJGroundLayer::create(1, 0);
    ground->setAnchorPoint(ccp(0.5f, 0));
    ground->setScale(0.85f);
    ground->setPosition(ccp(-569 / 2, 69));

    background = CCSprite::create("game_bg_01_001.png");
    background->setColor(ccc3(40, 125, 255));
    background->setPositionY(150);
    background->setScale(0.85f);

    clip->addChild(background, -2);
    clip->addChild(ground, 1);

    baseLayer->addChildAtPosition(langNode, Anchor::Top, ccp(0, -18));
    baseLayer->addChildAtPosition(clip, Anchor::Bottom);

    auto clipLine = CCLayerColor::create(ccc4(255, 255, 255, 255), size.x, 30);
    clipLine->setAnchorPoint(ccp(0.5f, 1));
    clipLine->ignoreAnchorPointForPosition(false);
    baseLayer->addChildAtPosition(clipLine, Anchor::Bottom, ccp(0, stencil->getContentHeight() + 1.5f));

    auto gameNode = CCMenu::create();
    gameNode->ignoreAnchorPointForPosition(false);
    gameNode->setContentSize(ccp(0, 0));
    gameNode->setPosition(ccp(0, 84.5f));

    auto creditsMenu = CCMenu::create();
    creditsMenu->ignoreAnchorPointForPosition(false);
    creditsMenu->setContentWidth(320);
    creditsMenu->setAnchorPoint(ccp(0.5f, 1));
    creditsMenu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setCrossAxisAlignment(AxisAlignment::Start)->setGap(15));
    creditsMenu->setZOrder(6);

    if (path != "none" && language.contains("contributors"))
    {
        for (auto contributor : language["contributors"].asArray().unwrap())
        {
            auto plr = PlayerObject::create(contributor["icon-id"].asUInt().unwrapOr(1), 2, nullptr, this, false);

            plr->setColor(GameManager::get()->colorForIdx(contributor["primary-col"].asInt().unwrapOr(0)));
            plr->setSecondColor(GameManager::get()->colorForIdx(contributor["secondary-col"].asInt().unwrapOr(0)));

            if (contributor["glow-enabled"].asBool().unwrapOr(false))
            {
                plr->enableCustomGlowColor(GameManager::get()->colorForIdx(contributor["glow-col"].asInt().unwrapOr(0)));
                plr->updateGlowColor();
            }

            plr->togglePlatformerMode(true);
            plr->m_regularTrail->setVisible(false);

            plr->runAction(CCSequence::create(CCDelayTime::create(0.37f), CCCallFunc::create(plr, callfunc_selector(TranslationCreditsLayer::spawnBigParticle)), nullptr));
            plr->runAction(CCSequence::create(CCDelayTime::create(0.73f), CCCallFunc::create(plr, callfunc_selector(TranslationCreditsLayer::spawnMediumParticle)), nullptr));
            plr->runAction(CCSequence::create(CCDelayTime::create(0.92f), CCCallFunc::create(plr, callfunc_selector(TranslationCreditsLayer::spawnSmallParticle)), nullptr));

            auto plrBtn = CCMenuItemSpriteExtra::create(plr, this, menu_selector(TranslationCreditsLayer::onKill));
            plrBtn->setContentSize(ccp(30, 30));
            plr->setPosition(plrBtn->getContentSize() / 2);

            plrBtn->setPosition(ccp((as<float>(rand()) / as<float>(RAND_MAX) * 2 - 1) * 120, 190));
            plrBtn->runAction(CCEaseBounceOut::create(CCMoveTo::create(1, ccp(plrBtn->getPositionX(), 0))));
            plrBtn->setTag(contributor["death-effect-id"].asUInt().unwrapOr(1));

            plrBtn->m_scaleMultiplier = 0.8f;

            gameNode->addChild(plrBtn);

            auto lbl = CCLabelBMFont::create(contributor["username"].asString().unwrapOr("Error").c_str(), "goldFont.fnt");
            lbl->limitLabelWidth(130, 1, 0);
            lbl->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            float s = lbl->getScale();
            
            auto btn = CCMenuItemSpriteExtra::create(lbl, this, menu_selector(TranslationCreditsLayer::onPlayerProfile));
            btn->setTag(contributor["account-id"].asUInt().unwrapOr(0));

            lbl->setScale(0);
            lbl->runAction(CCEaseElasticOut::create(CCScaleTo::create(1.25f, s)));

            creditsMenu->addChild(btn);
        }
    }

    creditsMenu->updateLayout();

    clip->addChild(gameNode);
    baseLayer->addChildAtPosition(creditsMenu, Anchor::Top, ccp(0, -45));

    if (path == "none")
    {
        auto thanks = CCLabelBMFont::create("Nobody to thank here.\nExcept you <3\nThanks for using QOLMod!", "bigFont.fnt");
        thanks->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);

        auto sprs = CCArrayExt<CCSprite*>(thanks->getChildren());

        sprs[28]->setColor(ccc3(255, 90, 90));
        sprs[29]->setColor(ccc3(255, 90, 90));
        sprs[30]->setColor(ccc3(255, 90, 90));
        sprs[31]->setColor(ccc3(255, 90, 90));
        sprs[32]->setColor(ccc3(255, 90, 90));
        sprs[33]->setColor(ccc3(255, 90, 90));

        sprs[51]->setColor(ccc3(255, 165, 75));
        sprs[52]->setColor(ccc3(255, 165, 75));
        sprs[53]->setColor(ccc3(255, 165, 75));
        sprs[54]->setColor(ccc3(255, 165, 75));
        sprs[55]->setColor(ccc3(255, 165, 75));
        sprs[56]->setColor(ccc3(255, 165, 75));

        auto menu = CCMenu::create();
        menu->setPosition(ccp(0, 150));
        menu->ignoreAnchorPointForPosition(false);
        menu->setScale(0.6f);
        menu->setContentSize(thanks->getContentSize());

        for (auto letter : CCArrayExt<CCSprite*>(thanks->getChildren()))
        {
            auto spr = CCSprite::createWithTexture(letter->getTexture());
            spr->setTextureRect(letter->getTextureRect());
            spr->setColor(letter->getColor());

            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TranslationCreditsLayer::onKill));
            btn->setTag(1);
            btn->m_scaleMultiplier = 0.8f;
            btn->setPosition(letter->getPosition());

            menu->addChild(btn);
        }

        clip->addChild(menu);
    }

    bool used = TranslationManager::get()->getLoadedLanguage() == language["display_name_english"];

    if (path == "none")
        used = !TranslationManager::get()->isLanguageLoaded();

    sprUse = ButtonSprite::create(used ? "Used" : "Use", "goldFont.fnt", used ? "GJ_button_05.png" : "GJ_button_01.png");
    btnUse = CCMenuItemSpriteExtra::create(sprUse, this, menu_selector(TranslationCreditsLayer::onUse));
    btnUse->setEnabled(!used);

    ok->setPositionX(ok->getPositionX() - btnUse->getScaledContentWidth() / 2);
    btnUse->setPosition(ccp(ok->getPositionX() + btnUse->getScaledContentWidth(), ok->getPositionY()));

    ok->setZOrder(420);
    baseLayer->addChild(btnUse, 69);
}

void TranslationCreditsLayer::onUse(CCObject* sender)
{
    if (path == "none")
        Client::get()->setLanguage("none");
    else
        Client::get()->setLanguage(path.filename().string());

    ok->setPositionX(ok->getPositionX() + btnUse->getScaledContentWidth() / 2);
    btnUse->removeFromParent();

    bool used = TranslationManager::get()->getLoadedLanguage() == language["display_name_english"];

    if (path == "none")
        used = !TranslationManager::get()->isLanguageLoaded();

    sprUse = ButtonSprite::create(used ? "Used" : "Use", "goldFont.fnt", used ? "GJ_button_05.png" : "GJ_button_01.png");
    btnUse = CCMenuItemSpriteExtra::create(sprUse, this, menu_selector(TranslationCreditsLayer::onUse));
    btnUse->setEnabled(!used);

    ok->setPositionX(ok->getPositionX() - btnUse->getScaledContentWidth() / 2);
    btnUse->setPosition(ccp(ok->getPositionX() + btnUse->getScaledContentWidth(), ok->getPositionY()));

    baseLayer->addChild(btnUse, 69);
}

void TranslationCreditsLayer::onPlayerProfile(CCObject* sender)
{
    ProfilePage::create(sender->getTag(), false)->show();
}

void TranslationCreditsLayer::onKill(CCObject* sender)
{
    FMODAudioEngine::get()->playEffect("explode_11.ogg");

    as<CCMenuItemSpriteExtra*>(sender)->setEnabled(false);
    as<CCMenuItemSpriteExtra*>(sender)->getNormalImage()->setVisible(false);

    auto death = PlayerDeathAnimation::createAndRun(sender->getTag());
    death->setPosition(as<CCMenuItemSpriteExtra*>(sender)->getContentSize() / 2);

    as<CCMenuItemSpriteExtra*>(sender)->addChild(death);

    background->runAction(CCSequence::create(CCTintTo::create(0.05f, 200, 0, 0), CCTintTo::create(0.5f, background->getColor().r, background->getColor().g, background->getColor().b), nullptr));

    for (auto grn : CCArrayExt<CCSprite*>(ground->getChildByType<CCSpriteBatchNode>(0)->getChildByType<CCSprite>(0)->getChildren()))
    {
        grn->runAction(CCSequence::create(CCTintTo::create(0.05f, 160, 0, 0), CCTintTo::create(0.5f, grn->getColor().r, grn->getColor().g, grn->getColor().b), nullptr));
    }

    if (sender->getTag() == 1)
    {
        killCount++;

        if (killCount == 50)
        {
            #ifndef GEODE_IS_MACOS

            AchievementNotifier::sharedState()->notifyAchievement("Waste of time", "Why would you waste your time doing that?", "diffIcon_04_btn_001.png", true);

            #endif
        }
    }
}

void TranslationCreditsLayer::spawnBigParticle()
{
    auto plr = reinterpret_cast<PlayerObject*>(this);

    auto part = GameToolbox::particleFromString(PARTICLE_HIT_BIG, nullptr, false);
    part->setDuration(1);
    part->setAutoRemoveOnFinish(true);
    part->setEmissionRate(-1);
    part->setPosition(plr->getParent()->getPosition() + ccp(0, -15));

    plr->getParent()->getParent()->addChild(part, 7);
}

void TranslationCreditsLayer::spawnMediumParticle()
{
    auto plr = reinterpret_cast<PlayerObject*>(this);

    auto part = GameToolbox::particleFromString(PARTICLE_HIT_MEDIUM, nullptr, false);
    part->setDuration(1);
    part->setAutoRemoveOnFinish(true);
    part->setEmissionRate(-1);
    part->setPosition(plr->getParent()->getPosition() + ccp(0, -15));

    plr->getParent()->getParent()->addChild(part, 6);
}

void TranslationCreditsLayer::spawnSmallParticle()
{
    auto plr = reinterpret_cast<PlayerObject*>(this);

    auto part = GameToolbox::particleFromString(PARTICLE_HIT_SMALL, nullptr, false);
    part->setDuration(1);
    part->setAutoRemoveOnFinish(true);
    part->setEmissionRate(-1);
    part->setPosition(plr->getParent()->getPosition() + ccp(0, -15));

    plr->getParent()->getParent()->addChild(part, 5);
}

TranslationCreditsLayer* TranslationCreditsLayer::create(matjson::Value language, std::filesystem::path path)
{
    auto pRet = new TranslationCreditsLayer();

    pRet->language = language;
    pRet->path = path;

    if (pRet && pRet->initWithSizeAndName(ccp(330, 280), ""))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

TranslationCreditsLayer* TranslationCreditsLayer::addToScene(matjson::Value language, std::filesystem::path path)
{
    auto pRet = TranslationCreditsLayer::create(language, path);

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}