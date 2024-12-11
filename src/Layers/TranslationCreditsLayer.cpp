#include "TranslationCreditsLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include "../UI/PlayerDeathAnimation.hpp"
#include "../Utils/TranslationManager.hpp"

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
    clip->addChild(ground, -1);

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
    creditsMenu->setLayout(AxisLayout::create()->setGrowCrossAxis(true)->setCrossAxisAlignment(AxisAlignment::Start));
    creditsMenu->setZOrder(6);

    if (path != "none")
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

#include "LanguageSelectNode.hpp"

class $modify (MeowMenuLayer, MenuLayer)
{
    void onMeow(CCObject*)
    {
        TranslationCreditsLayer::addToScene(file::readJson(Mod::get()->getResourcesDir() / "de-DE.json").unwrap(), Mod::get()->getResourcesDir() / "de-DE.json");
    }

    void onNyaa(CCObject*)
    {
        TranslationCreditsLayer::addToScene(matjson::parse("{ \"display_name_english\": \"Default\", \"display_name_native\": \"English\", \"contributors\": [] }").unwrapOr("{}"), "none");
    }

    void onCatgirl(CCObject*)
    {
        log::info("1: {}, 2: {}", TranslationManager::get()->isLanguageLoaded(), TranslationManager::get()->getLoadedLanguage());
    }

    void onFemboy(CCObject*)
    {
        LanguageSelectNode::addToScene();
    }

    void onMrrow(CCObject*)
    {
        matjson::Value obj;

        obj["display_name_english"] = "TODO";
        obj["display_name_native"] = "TODO";
        obj["contributors"] = obj.array();
        
        matjson::Value strings;

        for (auto window : Client::get()->windows)
        {
            strings[window->name] = "";
        }

        for (auto window : Client::get()->windows)
        {
            for (auto mod : window->modules)
            {
                strings[mod->name] = "";

                for (auto option : mod->options)
                {
                    if (!option)
                        continue;

                    strings[option->name] = "";
                }
            }
        }

        /*
        for (auto window : Client::get()->windows)
        {
            for (auto mod : window->modules)
            {
                strings[mod->description] = "";

                for (auto option : mod->options)
                {
                    if (!option)
                        continue;

                    strings[option->description] = "";
                }
            }
        }
        */

        obj["strings"] = strings;

        log::info("a:\n{}", obj.dump());
    }

    bool init()
    {
        MenuLayer::init();

        auto menu = CCMenu::create();

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_rewardBtn_001.png"), this, menu_selector(MeowMenuLayer::onMeow));
        btn->setPosition(ccp(100, 100));
        menu->addChild(btn);

        auto btn2 = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_epicCoin3_001.png"), this, menu_selector(MeowMenuLayer::onMrrow));
        btn2->setPosition(ccp(50, -50));
        menu->addChild(btn2);

        auto btn3 = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_editHSVBtn2_001.png"), this, menu_selector(MeowMenuLayer::onNyaa));
        btn3->setPosition(ccp(100, 50));
        menu->addChild(btn3);

        auto btn4 = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_like2Btn_001.png"), this, menu_selector(MeowMenuLayer::onCatgirl));
        btn4->setPosition(ccp(100, 0));
        menu->addChild(btn4);

        auto btn5 = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("label_of_001.png"), this, menu_selector(MeowMenuLayer::onFemboy));
        btn5->setPosition(ccp(150, -50));
        menu->addChild(btn5);

        this->addChild(menu, 6935434);

        Client::get()->getLanguages();

        return true;
    }
};