#pragma once

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client.h"
#include "../../Layers/IconOptionsLayer.h"

class EffectUI : public CCNode
{
    public:
        std::vector<SimplePlayer*> players;
        std::vector<SimplePlayer*> players2;
        std::vector<CCScale9Sprite*> chromas;
        std::vector<CCScale9Sprite*> pastels;
        std::vector<CCScale9Sprite*> fades;

        static inline int primary = 0;
        static inline int secondary = 0;
        static inline int glow = 0;
        static inline int trail = 0;
        static inline int waveTrail = 0;

        static inline std::array<float, 5> speeds = {1, 1, 1, 1, 1};
        static inline bool sameDual = false;

        static inline Hook* _hook = nullptr;

        static bool getIncompatibleModLoaded();
        static std::string getIncompatibleMods();

        static EffectUI* create() {
            EffectUI* ret = new EffectUI();
            if (ret && ret->init()) {
                ret->scheduleUpdate();
                ret->autorelease();
                return ret;
            } else {
                delete ret;
                ret = nullptr;
                return nullptr;
            }
        }

        static ccColor3B getColourForSelected(int mode, bool player2 = false);

        static void updateValues();

        void update(float delta)
        {
            auto color1 = getColourForSelected(0);
            auto color2 = getColourForSelected(1);
            auto glow = getColourForSelected(2);

            for (size_t i = 0; i < players.size(); i++)
            {
                players[i]->setColor(color1);
                players[i]->setSecondColor(color2);

                players[i]->enableCustomGlowColor(glow);
                players[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players[i]->updateColors();
            }

            for (size_t i = 0; i < players2.size(); i++)
            {
                if (sameDual)
                {
                    players2[i]->setColor(color1);
                    players2[i]->setSecondColor(color2);
                }
                else
                {
                    players2[i]->setColor(color2);
                    players2[i]->setSecondColor(color1);
                }

                players2[i]->enableCustomGlowColor(glow);
                players2[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players2[i]->updateColors();
            }
            
            for (size_t i = 0; i < chromas.size(); i++)
            {
                float v = ColourUtility::va;
                v *= Mod::get()->getSavedValue<float>(fmt::format("icon-effect-speed_{}", i), 1);

                chromas[i]->setColor(ColourUtility::getChromaColour(abs(v)));
            }

            for (size_t i = 0; i < chromas.size(); i++)
            {
                float v = ColourUtility::va;
                v *= Mod::get()->getSavedValue<float>(fmt::format("icon-effect-speed_{}", i), 1);

                pastels[i]->setColor(ColourUtility::getPastelColour(abs(v)));
            }

            for (size_t i = 0; i < fades.size(); i++)
            {
                auto fadeIn = fmt::format("fadeColour1{}", i);
                auto fadeOut = fmt::format("fadeColour2{}", i);

                ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn, {0, 0, 0});
                ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut, {255, 255, 255});

                float v = ColourUtility::va;
                v *= Mod::get()->getSavedValue<float>(fmt::format("icon-effect-speed_{}", i), 1);

                fades[i]->setColor(ColourUtility::lerpColour(in, out, (sinf(v * 3) + 1) / 2));
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
        
        static inline std::vector<CCNode*> selections = {};
        static inline std::vector<std::vector<CCNode*>> parts = {};

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
            IconOptionsLayer::addToScene(as<CCNode*>(sender)->getTag());
        }

        void changeDual(CCObject*)
        {
            bool sameDual = !Mod::get()->getSavedValue<bool>("same-dual");
            Mod::get()->setSavedValue<bool>("same-dual", sameDual);
            EffectUI::sameDual = sameDual;
        }

        void updateSelections()
        {
            selections[0]->setPosition(parts[0][Mod::get()->getSavedValue<int>(fmt::format("selColour{}", 0), 0)]->getPosition());
            selections[1]->setPosition(parts[1][Mod::get()->getSavedValue<int>(fmt::format("selColour{}", 1), 0)]->getPosition());
            selections[2]->setPosition(parts[2][Mod::get()->getSavedValue<int>(fmt::format("selColour{}", 2), 0)]->getPosition());
            selections[3]->setPosition(parts[3][Mod::get()->getSavedValue<int>(fmt::format("selColour{}", 3), 0)]->getPosition());
            selections[4]->setPosition(parts[4][Mod::get()->getSavedValue<int>(fmt::format("selColour{}", 4), 0)]->getPosition());

            EffectUI::updateValues();
        }

        void changeColour(CCObject* sender)
        {
            std::stringstream ss;
            ss << "selColour";
            ss << as<CCNode*>(sender)->getID();

            log::info("changing colour: {}", ss.str());

            Mod::get()->setSavedValue<int>(ss.str(), as<CCNode*>(sender)->getTag());

            updateSelections();
        }

        void generateType(CCMenu* menu, int type)
        {
            std::vector<CCNode*> p;

            CCPoint pos = ccp(225 / 2, (174 + (((type - 2) * -1) * 63.625f)) / 2);

            auto defSpr = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            defSpr->setContentWidth(defSpr->getContentWidth() * 1.8f);
            
            if (type == 0)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
            else if (type == 1)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
            else if (type == 2)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value()));
            else
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));

            auto d = CCLabelBMFont::create("Def", "bigFont.fnt");
            d->setOpacity(100);
            d->setScale(0.8f);
            defSpr->addChild(d);
            d->setPosition(defSpr->getContentSize() / 2);
            d->setPositionX(28.5f);
            d->setAnchorPoint(ccp(0.45f, 0.45f));

            defSpr->setScale(0.7f);

            auto dBtn = CCMenuItemSpriteExtra::create(defSpr, menu, menu_selector(IconEffects::changeColour));
            dBtn->setID(std::to_string(type));
            dBtn->setPosition(pos + ccp(30, 0));
            dBtn->setTag(0);
            p.push_back(dBtn);
            menu->addChild(dBtn);
            

            auto chSpr = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            chSpr->setContentWidth(chSpr->getContentWidth() * 1.8f);
            chSpr->setScale(defSpr->getScale());
            ui->chromas.push_back(chSpr);
            auto chBtn = CCMenuItemSpriteExtra::create(chSpr, menu, menu_selector(IconEffects::changeColour));
            chBtn->setID(std::to_string(type));
            chBtn->setPosition(dBtn->getPosition() + ccp(48, 0));
            //chBtn->addChild(CCSprite::createWithSpriteFrameName("GJ_select_001.png"));
            menu->addChild(chBtn);
            chBtn->setTag(1);
            p.push_back(chBtn);

            auto paSpr = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            paSpr->setContentWidth(paSpr->getContentWidth() * 1.8f);
            paSpr->setScale(defSpr->getScale());
            ui->pastels.push_back(paSpr);
            auto paBtn = CCMenuItemSpriteExtra::create(paSpr, menu, menu_selector(IconEffects::changeColour));
            paBtn->setID(std::to_string(type));
            paBtn->setPosition(chBtn->getPosition() + ccp(48, 0));
            menu->addChild(paBtn);
            paBtn->setTag(2);
            p.push_back(paBtn);

            auto faSpr = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            faSpr->setContentWidth(faSpr->getContentWidth() * 1.8f);
            faSpr->setScale(paSpr->getScale());
            ui->fades.push_back(faSpr);
            auto faBtn = CCMenuItemSpriteExtra::create(faSpr, menu, menu_selector(IconEffects::changeColour));
            faBtn->setID(std::to_string(type));
            faBtn->setPosition(paBtn->getPosition() + ccp(48, 0));
            menu->addChild(faBtn);
            faBtn->setTag(3);
            p.push_back(faBtn);

            auto faSSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
            auto faSBtn = CCMenuItemSpriteExtra::create(faSSpr, menu, menu_selector(IconEffects::onFadeSettings));
            faSBtn->setTag(type);
            faSBtn->setPosition(ccp(324, pos.y));
            faSSpr->setScale(0.55f);
            menu->addChild(faSBtn);

            parts.push_back(p);

            auto sel = CCScale9Sprite::createWithSpriteFrameName("GJ_select_001.png");
            sel->setContentSize(faSpr->getContentSize() + ccp(-5, 0));
            sel->setScale(0.85f);
            menu->addChild(sel);
            selections.push_back(sel);
        }

        void cocosCreate(CCMenu* menu)
        {
            selections.clear();
            parts.clear();

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

            auto split = CCSprite::createWithSpriteFrameName("floorLine_001.png");
            split->setPosition(ccp(225, back2->getContentHeight() / 2));
            split->setScaleY(2);
            split->setScaleX(0.7f);
            split->setRotation(-90);
            split->setOpacity(100);
            split->setAnchorPoint(ccp(0.5f, 0));

            auto label = CCLabelBMFont::create("Primary:\n\nSecondary:\n\nGlow:\n\nTrail:\n\nWave Trail:", "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setPosition(ccp(10, back2->getContentHeight() / 2));
            back2->addChild(label);

            auto blend = ccBlendFunc({GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA});
            split->setBlendFunc(blend);

            back2->addChild(split);

            generateType(menu, 0);
            generateType(menu, 1);
            generateType(menu, 2);
            generateType(menu, 3);
            generateType(menu, 4);

            updateSelections();
        }
};