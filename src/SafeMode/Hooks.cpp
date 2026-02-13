#include "Hooks.hpp"
#include "SafeMode.hpp"
#include "Modules/SafeMode.hpp"
#include "../GUI/BetterButtonSprite.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterAlertLayer.hpp"
#include "UI/CheatIndicator.hpp"
#include "Modules/AutoSafeMode.hpp"
#include "Modules/SafeMode.hpp"

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

void SafePlayLayer::postUpdate(float dt)
{
    PlayLayer::postUpdate(dt);

    for (auto mod : Module::getAll())
    {
        if (mod->getSafeModeTrigger() == SafeModeTrigger::Custom)
        {
            if (mod->getSafeModeCustom()())
            {
                SafeMode::get()->addMessage(SafeModeTrigger::Attempt, fmt::format("<cc>{}</c> enabled for <co>this attempt</c>", mod->getName()));
                SafeMode::get()->isAttemptCheated = true;
            }
        }
    }
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

    auto ci = CheatIndicator::create();
    ci->setPosition(CCDirector::get()->getWinSize() / 2 + (m_listLayer->getContentSize() / 2 * ccp(-1, 1)) + ccp(10, -14));
    ci->setScale(1.25f);
    
    m_fields->ci = ci;
    m_mainLayer->addChild(ci);

    this->schedule(schedule_selector(SafeEndLevelLayer::updateCheatIndicator));
    updateCheatIndicator(0);
    
    if (SafeMode::get()->shouldDisableLevelProgress() && (AutoSafeMode::get()->getRealEnabled() || ForceSafeMode::get()->getRealEnabled()))
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

    BetterAlertLayer::create(loc->getLocalisedString("names/safe-mode").c_str(), SafeMode::get()->getCombinedMessages(), loc->getLocalisedString("ui/ok-button").c_str())->show();
}

void SafeEndLevelLayer::updateCheatIndicator(float)
{
    m_fields->ci->setVisible(SafeModeEndScreenCheatIndicator::get()->getRealEnabled());
}

void SafeEndLevelLayer::playCoinEffect(float duration)
{
    if (SafeMode::get()->shouldDisableLevelProgress())
        return;

    EndLevelLayer::playCoinEffect(duration);
}