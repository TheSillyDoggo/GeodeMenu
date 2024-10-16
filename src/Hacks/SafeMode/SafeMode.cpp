#include "SafeMode.hpp"

SafeMode* SafeMode::get()
{
    if (!instance)
        instance = new SafeMode();

    return instance;
}

void SafeMode::resetOnLevelLoad()
{
    hackedLevelLoad = false;
    hackedAttempt = false;
    hackedAttemptReal = false;

    loadReasons.clear();

    updateIndicator();
}

void SafeMode::resetOnNewAttempt()
{
    hackedAttempt = Client::GetModuleEnabled("auto-safe");
    hackedAttemptReal = Client::GetModuleEnabled("auto-safe");

    bool autosafe = Client::GetModuleEnabled("auto-safe-mode");

    reasons.clear();

    for (auto hack : hacks)
    {
        if (Client::GetModuleEnabled(hack))
        {
            if (autosafe)
                hackedAttempt = true;

            hackedAttemptReal = true;

            reasons.push_back(fmt::format("{} enabled", Client::GetModule(hack)->name));
        }
    }

    updateIndicator();
}

void SafeMode::setHackedLoad(std::string reason)
{
    hackedLevelLoad = true;

    loadReasons.push_back(reason);

    updateIndicator();
}

void SafeMode::setHackedAttempt(std::string reason)
{
    if (Client::GetModuleEnabled("auto-safe-mode"))
        hackedAttempt = true;
    
    reasons.push_back(reason);

    hackedAttemptReal = true;
    updateIndicator();
}

void SafeMode::addDelegateToModules()
{
    for (auto hack : hacks)
    {
        Client::GetModule(hack)->delegate = new HackModuleDelegate();
        Client::GetModule(hack)->delegate->_module = Client::GetModule(hack);
    }

    SpeedhackEnabled::instance->delegate = new SpeedhackDelegate();
    SpeedhackTop::instance->delegate = new SpeedhackDelegate();

    updateSpeedhackShouldKick();
}

ccColor3B SafeMode::colourForState()
{
    if (Client::GetModuleEnabled("safe-mode"))
        return ccc3(255, 255, 0);

    if (hackedAttemptReal || hackedLevelLoad || speedhackKick)
        return ccc3(255, 0, 0);

    return ccc3(0, 255, 0);
}

bool SafeMode::shouldKickFromLevel()
{
    return hackedAttempt || hackedLevelLoad || speedhackKick || Client::GetModuleEnabled("safe-mode");
}

std::vector<std::pair<bool, std::string>> SafeMode::getReasons()
{
    std::vector<std::pair<bool, std::string>> v;

    for (auto reason : loadReasons)
        v.push_back(std::make_pair(true, reason));

    for (auto reason : reasons)
        v.push_back(std::make_pair(false, reason));

    return v;
}

void SafeMode::updateIndicator()
{
    #ifdef STATUS_TEXTS

    //if (auto a = StatusNode::get())
    //{
    //    if (auto l = a->sLabels[0])
    //        l->setColor(colourForState());
//
//        a->update(-1);
//        a->reorderSides();
//        a->reorderPosition();
//    }

    #endif
}

void SafeMode::updateSpeedhackShouldKick()
{
    speedhackKick = Client::GetModuleEnabled("auto-safe-mode") ? (SpeedhackEnabled::instance->enabled ? (SpeedhackTop::instance->getFloatValue() < 1) : false) : false;
}

void HackModuleDelegate::onModuleChanged(bool enabled)
{
    SafeMode::get()->setHackedAttempt(fmt::format("{} enabled", as<Module*>(_module)->name));
    SafeMode::get()->updateIndicator();
}

void SpeedhackDelegate::onModuleChanged(bool enabled)
{
    SafeMode::get()->updateSpeedhackShouldKick();

    if (SafeMode::get()->speedhackKick)
        SafeMode::get()->setHackedAttempt("Speedhack above 1.0");
    
    SafeMode::get()->updateIndicator();
}

// hooks :3

bool SafePlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    SafeMode::get()->resetOnLevelLoad();
    SafeMode::get()->resetOnNewAttempt();

    if (Client::GetModuleEnabled("force-plat"))
        SafeMode::get()->setHackedLoad("Force Platformer");

    if (Client::GetModuleEnabled("show-triggers"))
        SafeMode::get()->setHackedLoad("Show Triggers");

    if (Client::GetModuleEnabled("show-layout"))
        SafeMode::get()->setHackedLoad("Show Layout");

    return true;
}

void SafePlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    SafeMode::get()->resetOnNewAttempt();
}

void SafePlayLayer::levelComplete()
{
    auto kick = SafeMode::get()->shouldKickFromLevel();
    SafeMode::get()->safeEndScreen = kick;
    auto v = m_isTestMode;

    if (kick)
        m_isTestMode = true;

    PlayLayer::levelComplete();

    m_isTestMode = v;
}

void SafePlayLayer::showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5)
{
    if (!SafeMode::get()->shouldKickFromLevel())
        PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
}

void SafeGJGameLevel::savePercentage(int p0, bool p1, int p2, int p3, bool p4)
{
    if (!SafeMode::get()->shouldKickFromLevel())
        GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
}

void SafeEndLevelLayer::customSetup()
{
    EndLevelLayer::customSetup();

    if (SafeMode::get()->safeEndScreen)
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

        if (hasHiddenCoin)
        {
            auto lbl = CCLabelBMFont::create("Safe Mode :3", "bigFont.fnt");
            lbl->setID("safe-mode-text"_spr);
            lbl->setScale(0.7f);

            // i dont even know
            lbl->setPositionY((m_playLayer->m_level->isPlatformer() ? 147 - (32.5 * 0.5) : 95) - 320 / 2 + CCDirector::get()->getWinSize().height / 2);
            lbl->setPositionX(CCDirector::get()->getWinSize().width / 2);

            m_mainLayer->addChild(lbl);
        }
        else
        {
            if (auto area = getChildOfType<TextArea>(m_mainLayer, -1))
            {
                area->setString("Safe Mode :3");
                area->setScale(0.7f);
            }
            else if (auto lbl = getChildOfType<CCLabelBMFont>(m_mainLayer, -1))
            {
                lbl->setString("Safe Mode :3");
                lbl->setScale(0.7f);
            }
        }

        std::stringstream safeModeStream;

        for (auto reason : SafeMode::get()->getReasons())
        {
            safeModeStream << (reason.first ? "<cd>" : "<cs>");
            safeModeStream << reason.second;
            safeModeStream << "</c>\n";
        }

        auto infoMenu = CCMenu::create();
        infoMenu->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(-181.5f, 126.5f));

        auto infoBtn = InfoAlertButton::create("Safe Mode", safeModeStream.str().c_str(), 0.75f);
        infoMenu->addChild(infoBtn);

        m_mainLayer->addChild(infoMenu);
    }
}