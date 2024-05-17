#pragma once

#include "../Client/Client.h"

class EffectUI : public CCNode
{
    public:
        std::vector<SimplePlayer*> players;
        std::vector<SimplePlayer*> players2;
        std::vector<CCSprite*> chromas;
        std::vector<CCSprite*> pastels;
        std::vector<CCSprite*> fades;

        bool init()
        {
            if (!CCNode::init())
                return false;

            this->scheduleUpdate();

            return true;
        }

        static EffectUI* create() {
            EffectUI* ret = new EffectUI();
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            } else {
                delete ret;
                ret = nullptr;
                return nullptr;
            }
        }

        static ccColor3B getColourForSelected(int mode);

        void update(float delta) {
            for (size_t i = 0; i < players.size(); i++)
            {
                players[i]->setColor(getColourForSelected(0));
                players[i]->setSecondColor(getColourForSelected(1));

                players[i]->enableCustomGlowColor(getColourForSelected(2));
                players[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players[i]->updateColors();
            }

            for (size_t i = 0; i < players2.size(); i++)
            {
                if (Mod::get()->getSavedValue<bool>("same-dual"))
                {
                    players2[i]->setColor(getColourForSelected(0));
                    players2[i]->setSecondColor(getColourForSelected(1));
                }
                else
                {
                    players2[i]->setColor(getColourForSelected(1));
                    players2[i]->setSecondColor(getColourForSelected(0));
                }

                players2[i]->enableCustomGlowColor(getColourForSelected(2));
                players2[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players2[i]->updateColors();
            }
            
            for (size_t i = 0; i < chromas.size(); i++)
            {
                chromas[i]->setColor(ColourUtility::getChromaColour());
            }

            for (size_t i = 0; i < chromas.size(); i++)
            {
                pastels[i]->setColor(ColourUtility::getPastelColour());
            }

            for (size_t i = 0; i < fades.size(); i++)
            {
                std::stringstream fadeIn;
                fadeIn << "fadeColour1";
                fadeIn << i;

                std::stringstream fadeOut;
                fadeOut << "fadeColour2";
                fadeOut << i;

                ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
                ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

                fades[i]->setColor(ColourUtility::lerpColour(in, out, (sinf(ColourUtility::va * 3) + 1) / 2));
            }
        }
};

class IconEffects : public Window
{
    public:
        IconEffects()
        {
            name = "Icon Effects";
            id = "icon-effects";
        }

        EffectUI* ui = nullptr;        

        SimplePlayer* getPlayer(IconType type, EffectUI* ui, bool second = false)
        {
            int id = GameManager::get()->m_playerFrame.value();

            if (type == IconType::Ship)
                id = GameManager::get()->m_playerShip.value();
            if (type == IconType::Ball)
                id = GameManager::get()->m_playerBall.value();
            if (type == IconType::Ufo)
                id = GameManager::get()->m_playerBird.value();
            if (type == IconType::Wave)
                id = GameManager::get()->m_playerDart.value();
            if (type == IconType::Robot)
                id = GameManager::get()->m_playerRobot.value();
            if (type == IconType::Spider)
                id = GameManager::get()->m_playerSpider.value();
            if (type == IconType::Swing)
                id = GameManager::get()->m_playerSwing.value();
            if (type == IconType::Jetpack)
                id = GameManager::get()->m_playerJetpack.value();


            auto plr = SimplePlayer::create(id);
            plr->updatePlayerFrame(id, type);
            plr->setScale(1.4f);
            if (second)
                ui->players2.push_back(plr);
            else
                ui->players.push_back(plr);

            return plr;
        }

        void onFadeSettings(CCObject* sender)
        {
            CCScene::get()->addChild(SetupFadeSetting::create(as<CCNode*>(sender)->getTag()), 99999);
        }

        void changeDual(CCObject*)
        {
            Mod::get()->setSavedValue<bool>("same-dual", !Mod::get()->getSavedValue<bool>("same-dual"));
        }

        void changeColour(CCObject* sender)
        {
            std::stringstream ss;
            ss << "selColour";
            ss << as<CCNode*>(sender)->getID();

            log::info("changing colour: {}", ss.str());

            Mod::get()->setSavedValue<int>(ss.str(), as<CCNode*>(sender)->getTag());
        }

        void generateType(CCMenu* menu, int type)
        {
            CCPoint pos = ccp((menu->getContentSize().width / 3) * type, 0) + ccp(10, -40);
            float width = menu->getContentSize().width / 4;

            auto defSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            
            if (type == 0)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
            else if (type == 1)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
            else
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value()));

            auto d = CCLabelBMFont::create("D", "bigFont.fnt");
            d->setOpacity(100);
            defSpr->addChild(d);
            d->setPosition(defSpr->getContentSize() / 2);
            d->setAnchorPoint(ccp(0.45f, 0.45f));

            defSpr->setScale(0.7f);

            auto dBtn = CCMenuItemSpriteExtra::create(defSpr, menu, menu_selector(IconEffects::changeColour));
            dBtn->setID(std::to_string(type));
            dBtn->setPosition(pos + ccp(width, menu->getContentSize().height - 80));
            menu->addChild(dBtn);
            dBtn->setTag(0);

            auto chSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            chSpr->setScale(defSpr->getScale());
            ui->chromas.push_back(chSpr);
            auto chBtn = CCMenuItemSpriteExtra::create(chSpr, menu, menu_selector(IconEffects::changeColour));
            chBtn->setID(std::to_string(type));
            chBtn->setPosition(dBtn->getPosition() + ccp(0, -30));
            //chBtn->addChild(CCSprite::createWithSpriteFrameName("GJ_select_001.png"));
            menu->addChild(chBtn);
            chBtn->setTag(1);

            auto paSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            paSpr->setScale(defSpr->getScale());
            ui->pastels.push_back(paSpr);
            auto paBtn = CCMenuItemSpriteExtra::create(paSpr, menu, menu_selector(IconEffects::changeColour));
            paBtn->setID(std::to_string(type));
            paBtn->setPosition(chBtn->getPosition() + ccp(0, -30));
            menu->addChild(paBtn);
            paBtn->setTag(2);

            auto faSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            faSpr->setScale(paSpr->getScale());
            ui->fades.push_back(faSpr);
            auto faBtn = CCMenuItemSpriteExtra::create(faSpr, menu, menu_selector(IconEffects::changeColour));
            faBtn->setID(std::to_string(type));
            faBtn->setPosition(paBtn->getPosition() + ccp(0, -30));
            menu->addChild(faBtn);
            faBtn->setTag(3);

            auto faSSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
            auto faSBtn = CCMenuItemSpriteExtra::create(faSSpr, menu, menu_selector(IconEffects::onFadeSettings));
            faSBtn->setTag(type);
            faSBtn->setPosition(faBtn->getPosition() + ccp(-25, 0));
            faSSpr->setScale(0.45f);
            menu->addChild(faSBtn);

            auto def = CCLabelBMFont::create("Default:", "bigFont.fnt");
            def->setPosition(pos + ccp(-6, dBtn->getPositionY() + 40));
            def->setOpacity(100);
            def->setAnchorPoint(ccp(0, 0.5f));
            def->setScale(0.5f);
            menu->addChild(def);

            auto ch = CCLabelBMFont::create("Chroma:", "bigFont.fnt");
            ch->setPosition(pos + ccp(-6, chBtn->getPositionY() + 40));
            ch->setOpacity(100);
            ch->setAnchorPoint(ccp(0, 0.5f));
            ch->setScale(0.5f);
            menu->addChild(ch);

            auto pa = CCLabelBMFont::create("Pastel:", "bigFont.fnt");
            pa->setPosition(pos + ccp(-6, paBtn->getPositionY() + 40));
            pa->setOpacity(100);
            pa->setAnchorPoint(ccp(0, 0.5f));
            pa->setScale(0.5f);
            menu->addChild(pa);

            auto fa = CCLabelBMFont::create("Fade:", "bigFont.fnt");
            fa->setPosition(pos + ccp(-6, faBtn->getPositionY() + 40));
            fa->setOpacity(100);
            fa->setAnchorPoint(ccp(0, 0.5f));
            fa->setScale(0.5f);
            menu->addChild(fa);
        }

        float x = 0;
        int selectedTab = 0;

        void onTabChanged(CCObject* sender)
        {
            auto v = as<CCMenuItemToggler*>(sender);
            selectedTab = v->getTag();

            for (size_t i = 0; i < v->getParent()->getChildrenCount(); i++)
            {
                if (auto obj = as<CCMenuItemToggler*>(v->getParent()->getChildren()->objectAtIndex(i)))
                {
                    if (obj != v)
                    {
                        obj->toggle(false);
                    }

                    if (!obj->isToggled() && selectedTab == i)
                    {
                        obj->toggle(true);
                        obj->setScale(1);
                    }
                }
            }

            log::info("ta{}", selectedTab);
        }

        CCMenuItemToggler* createBtn(const char* text)
        {
            auto v = CCMenuItemToggler::create(ButtonSprite::create(text, "goldFont.fnt", "GJ_button_04.png"), ButtonSprite::create(text, "goldFont.fnt", "GJ_button_05.png"), nullptr, menu_selector(IconEffects::onTabChanged));
            v->setPositionX(x);
            v->setAnchorPoint(ccp(0, 0.5f));
            v->setTag(selectedTab);

            x += v->getContentWidth() + 10;

            if (std::string(text) == std::string("Primary 1"))
                v->toggle(true);

            selectedTab++;

            return v;
        }

        void cocosCreate(CCMenu* menu)
        {
            x = 0;
            selectedTab = 0;

            ui = EffectUI::create();
            menu->addChild(ui);

            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(ccp(menu->getContentSize().width, 80) / 0.5f);
            back->setPosition(ccp(0, menu->getContentSize().height));
            back->setAnchorPoint(ccp(0, 1));
            back->setScale(0.5f);
            back->setOpacity(100);
            menu->addChild(back);

            auto back2 = CCScale9Sprite::create("square02_small.png");
            back2->setContentSize(ccp(menu->getContentSize().width, menu->getContentSize().height - 80 - 6) / 0.5f);
            back2->setPosition(ccp(0, 0));
            back2->setAnchorPoint(ccp(0, 0));
            back2->setScale(0.5f);
            back2->setOpacity(100);
            menu->addChild(back2);

            #pragma region Normal

            auto m = CCMenu::create();
            m->setAnchorPoint(ccp(0, 0));
            m->ignoreAnchorPointForPosition(false);
            m->setPosition(ccp(10, 10));
            m->setPositionY(back->getContentSize().height - 40);
            m->setContentSize(back->getContentSize() - ccp(120, 20));
            back->addChild(m);

            m->addChild(getPlayer(IconType::Cube, ui));
            m->addChild(getPlayer(IconType::Ship, ui));
            m->addChild(getPlayer(IconType::Ball, ui));
            m->addChild(getPlayer(IconType::Ufo, ui));
            m->addChild(getPlayer(IconType::Wave, ui));
            m->addChild(getPlayer(IconType::Robot, ui));
            m->addChild(getPlayer(IconType::Spider, ui));
            m->addChild(getPlayer(IconType::Swing, ui));
            m->addChild(getPlayer(IconType::Jetpack, ui));

            m->setLayout(RowLayout::create()->setGap(64)->setAutoScale(false)->setGrowCrossAxis(false));
            m->updateLayout();

            #pragma endregion

            #pragma region Dual

            auto m2 = CCMenu::create();
            m2->setAnchorPoint(ccp(0, 0));
            m2->ignoreAnchorPointForPosition(false);
            m2->setPosition(ccp(10, 10));
            m2->setPositionY(40);
            m2->setContentSize(back->getContentSize() - ccp(120, 20));
            back->addChild(m2);

            m2->addChild(getPlayer(IconType::Cube, ui, true));
            m2->addChild(getPlayer(IconType::Ship, ui, true));
            m2->addChild(getPlayer(IconType::Ball, ui, true));
            m2->addChild(getPlayer(IconType::Ufo, ui, true));
            m2->addChild(getPlayer(IconType::Wave, ui, true));
            m2->addChild(getPlayer(IconType::Robot, ui, true));
            m2->addChild(getPlayer(IconType::Spider, ui, true));
            m2->addChild(getPlayer(IconType::Swing, ui, true));
            m2->addChild(getPlayer(IconType::Jetpack, ui, true));

            m2->setLayout(RowLayout::create()->setGap(64)->setAutoScale(false)->setGrowCrossAxis(false));
            m2->updateLayout();

            #pragma endregion

            auto sameTitle = CCLabelBMFont::create("Same\nDual\nColour", "bigFont.fnt", 110, CCTextAlignment::kCCTextAlignmentCenter);
            sameTitle->setPosition(menu->getContentSize() + ccp(-25, -20));
            sameTitle->setScale(0.3f);
            menu->addChild(sameTitle);

            auto sameDual = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(IconEffects::changeDual), 0.6f);
            sameDual->setPosition(menu->getContentSize() + ccp(-25, -50));
            sameDual->toggle(Mod::get()->getSavedValue<bool>("same-dual"));
            menu->addChild(sameDual);

            for (size_t i = 0; i < 2; i++)
            {
                auto split = CCSprite::createWithSpriteFrameName("floorLine_001.png");
                split->setPosition(ccp(back2->getContentSize().width / 3 * (i + 1), back2->getContentSize().height / 2));
                split->setScaleY(2);
                split->setScaleX(0.7f);
                split->setRotation(-90);
                split->setOpacity(100);
                split->setAnchorPoint(ccp(0.5f, 0));

                auto blend = ccBlendFunc({GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA});
                split->setBlendFunc(blend);

                back2->addChild(split);
            }

            for (size_t i = 0; i < 3; i++)
            {
                auto title = CCLabelBMFont::create(i == 0 ? "Primary" : (i == 1 ? "Secondary" : "Glow"), "bigFont.fnt");
                title->setPosition(ccp((back2->getContentSize().width / 3) * i + ((back2->getContentSize().width / 3) / 2), back2->getContentSize().height - 20));

                back2->addChild(title);

                generateType(menu, i);
            }

            return;

            auto menu2 = CCMenu::create();
            menu2->setContentHeight(0);
            menu2->setAnchorPoint(ccp(0, 0.5f));
            menu2->setScale(0.5f);
            menu2->setPosition(ccp(10, 160));

            menu2->addChild(createBtn("Primary"));
            menu2->addChild(createBtn("Secondary"));
            menu2->addChild(createBtn("Glow"));
            menu2->addChild(createBtn("Trail"));
            menu2->addChild(createBtn("Wave Trail"));

            menu->addChild(menu2);

            selectedTab = 0;
        }
};