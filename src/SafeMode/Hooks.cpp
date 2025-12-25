#include "Hooks.hpp"
#include "SafeMode.hpp"
#include "Modules/SafeMode.hpp"
#include "../GUI/BetterButtonSprite.hpp"
#include "../Localisation/LocalisationManager.hpp"

bool SafePlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    SafeMode::get()->onLevelLoad();

    return true;
}

void SafePlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    SafeMode::get()->onLevelNewAttempt();
}

void SafePlayLayer::levelComplete()
{
    bool v = m_isTestMode;

    m_isTestMode = SafeMode::get()->shouldDisableLevelProgress();

    PlayLayer::levelComplete();

    m_isTestMode = v;
}

void SafePlayLayer::showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
}

void SafeGJGameLevel::onModify(auto& self)
{
    (void)self.setHookPriorityPost("GJGameLevel::savePercentage", Priority::First);
}

void SafeGJGameLevel::savePercentage(int p0, bool p1, int p2, int p3, bool p4)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
}

void SafeEndLevelLayer::customSetup()
{
    EndLevelLayer::customSetup();
    
    if (SafeMode::get()->shouldDisableLevelProgress())
    {
        bool hasHiddenCoin = false;

        for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren()))
        {
            if (auto sprite = typeinfo_cast<CCSprite*>(child))
            {
                for (auto f : { "secretCoinUI_001.png", "secretCoinUI2_001.png", "secretCoin_b_01_001.png" })
                {
                    if (isSpriteFrameName(sprite, f))
                    {
                        hasHiddenCoin = true;
                        sprite->setVisible(false);
                    }
                }
            }
        }

        for (auto coin : CCArrayExt<CCNode*>(m_coinsToAnimate))
        {
            coin->setVisible(false);
        }

        CCNode* n = nullptr;

        if (SafeModeChangeText::get()->getRealEnabled())
        {
            auto str = SafeModeText::get()->getString();

            if (hasHiddenCoin)
            {
                auto lbl = CCLabelBMFont::create(str.c_str(), "bigFont.fnt");
                lbl->setID("safe-mode-text"_spr);
                lbl->setScale(0.7f);

                // i dont even know
                lbl->setPositionY((m_playLayer->m_level->isPlatformer() ? 147 - (32.5 * 0.5) : 95) - 320 / 2 + CCDirector::get()->getWinSize().height / 2);
                lbl->setPositionX(CCDirector::get()->getWinSize().width / 2);
                lbl->setPositionY(lbl->getPositionY() + 18);

                n = lbl;

                m_mainLayer->addChild(lbl);
            }
            else
            {
                if (auto area = m_mainLayer->getChildByType<TextArea>(-1))
                {
                    area->setString(str);
                    area->setScale(0.7f);
                    area->setPositionY(area->getPositionY() + 18);

                    n = area;
                }
                else if (auto lbl = m_mainLayer->getChildByType<CCLabelBMFont>(-1))
                {
                    lbl->setString(str.c_str());
                    lbl->setScale(0.7f);
                    lbl->setPositionY(lbl->getPositionY() + 18);
                    
                    n = lbl;
                }
            }
        }

        auto infoMenu = CCMenu::create();
        infoMenu->setScale(0.65f);
        infoMenu->setAnchorPoint(ccp(0, 0));
        infoMenu->setPosition((n ? n->getPosition() : CCPointMake(CCDirector::get()->getWinSize().width, CCDirector::get()->getWinSize().height) / 2) + ccp(0, -37.5f));
        infoMenu->setID("info-menu"_spr);

        auto infoBtn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(ccp(180, 30), LocalisationManager::get()->getLocalisedString("ui/safe-mode-view-reason-button"), "bigFont.fnt", "GJ_button_04.png"), this, menu_selector(SafeEndLevelLayer::onViewReasons));
        infoBtn->m_scaleMultiplier = 1.1f;
        infoMenu->addChild(infoBtn);

        m_mainLayer->addChild(infoMenu);
    }
}

void SafeEndLevelLayer::onViewReasons(CCObject* sender)
{
    auto loc = LocalisationManager::get();

    FLAlertLayer::create(loc->getLocalisedString("names/safe-mode").c_str(), SafeMode::get()->getCombinedMessages(), loc->getLocalisedString("ui/ok-button").c_str())->show();
}

void SafeEndLevelLayer::playCoinEffect(float duration)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    EndLevelLayer::playCoinEffect(duration);
}