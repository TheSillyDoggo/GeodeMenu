#include <Geode/Geode.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../Client/Client.h"

bool hasHackedAttempt = false;

class HackModuleDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        if (Client::GetModuleEnabled("auto-safe-mode"))
            hasHackedAttempt = true;
        else
            hasHackedAttempt = false;


    }
};

std::vector<std::string> hacks = {
    "speedhack-enabled",
    "force-plat",
    "noclip",
    "instant",
    "no-reverse",
    "no-static",
    "no-shaders",
};

void Client::onPostSetup()
{
    log::info("post setup");

    for (auto mod : hacks)
    {
        Client::GetModule(mod)->delegate = new HackModuleDelegate();
    }
}

void updateSafemode()
{
    if (Client::GetModuleEnabled("auto-safe-mode"))
    {
        for (auto mod : hacks)
        {
            if (Client::GetModule(mod)->enabled)
                hasHackedAttempt = true;
        }
    }

    hasHackedAttempt = (Client::GetModule("safe-mode")->enabled) ? true : hasHackedAttempt;
}

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        hasHackedAttempt = false;
        updateSafemode();

        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }

    TodoReturn levelComplete()
    {
        log::info("hacked attempt: {}", hasHackedAttempt);

        if (hasHackedAttempt)
        {
            if (m_isPracticeMode)
            {
                //todo: implement :3
            }
            else
            {
                showCompleteEffect();
            }

            return;
        }

        PlayLayer::levelComplete();

        //updateSafemode();

        //this->onQuit();
        //FMODAudioEngine::sharedEngine()->playEffect("quitSound_01.ogg");
    }

    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        if (!hasHackedAttempt)
            return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }

    void resetLevel()
    {
        PlayLayer::resetLevel();

        updateSafemode();
        hasHackedAttempt = Client::GetModule("safe-mode")->enabled;
    }
};

class $modify(GJGameLevel) {
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        updateSafemode();

        if (!hasHackedAttempt)
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
    }
};

class $modify(EndLevelLayerExt, EndLevelLayer)
{
    void onInfo(CCObject*)
    {
        std::stringstream ss;

        std::vector<std::string> hs = {};

        for (auto mod : hacks)
        {
            if (Client::GetModule(mod)->enabled)
                hs.push_back(Client::GetModule(mod)->name);
        }

        if (hs.size() != 0)
        {
            ss << "<ca>Faulty Mods:</c>";
            ss << "\n";

            for (auto mod : hs)
            {
                ss << "<cp>-</c> ";
                ss << mod;
                ss << "\n";
            }

            ss << "\n";
        }

        ss << "<cg>Green - No Cheats are enabled</c>\n";
        ss << "<cy>Yellow - Safe Mode is enabled</c>\n";
        ss << "<cr>Red - Cheats are enabled";
        if (hs.size() != 0)
            ss << ", See above.";
        ss << "</c>";

        auto alert = FLAlertLayer::create(nullptr, "Cheat Indicator", ss.str(), "OK", nullptr, 300, false, 300, 0.7f);
        alert->show();
    }

    ccColor3B getColour()
    {
        if (Client::GetModule("safe-mode")->enabled)
            return ccc3(255, 255, 0);

        if (hasHackedAttempt)
            return ccc3(255, 0, 0);

        return ccc3(0, 255, 0);
    }

	void customSetup()
	{
		EndLevelLayer::customSetup();

        auto menu = CCMenu::create();
        menu->setPosition(CCDirector::get()->getWinSize() / 2);

        auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(EndLevelLayerExt::onInfo));
        info->m_baseScale = 0.75f;
        info->setScale(info->m_baseScale);

        info->setPosition(ccp(-181.5f, 126.5f));
        menu->addChild(info);

        auto dot = CCSprite::createWithSpriteFrameName("uiDot_001.png");
        dot->setPosition(ccp(-166, 100));
        dot->setScale(1.5f);
        dot->setColor(getColour());
        menu->addChild(dot);

        getChildOfType<CCLayer>(this, 0)->addChild(menu);
    }
};