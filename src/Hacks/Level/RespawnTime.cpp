#include "../../Client/InputModule.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class CustomRespawnTime : public Module
{
    public:
        MODULE_SETUP(CustomRespawnTime)
        {
            setName("Custom Respawn Time");
            setID("custom-respawn-time");
            setCategory("Level");
            setDescription("Change how long it takes to respawn");
        }
};

class InstantRestart : public Module
{
    public:
        MODULE_SETUP(InstantRestart)
        {
            setName("Instant Restart");
            setID("instant-restart");
            setCategory("Level");
            setDescription("Instantly restart a level when you die");
        }
};

class CustomRespawnNormal : public InputModule
{
    public:
        MODULE_SETUP(CustomRespawnNormal)
        {
            setName("Normal");
            setID("instant-restart/normal");
            setDescription("");

            setDefaultString("1");
            setPlaceholderString("Time");
            setHint("sec");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class CustomRespawnPractice : public InputModule
{
    public:
        MODULE_SETUP(CustomRespawnPractice)
        {
            setName("Practice");
            setID("instant-restart/practice");
            setDescription("");

            setDefaultString("0.5");
            setPlaceholderString("Time");
            setHint("sec");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(CustomRespawnTime);
SUBMIT_HACK(InstantRestart);
SUBMIT_OPTION(CustomRespawnTime, CustomRespawnNormal);
SUBMIT_OPTION(CustomRespawnTime, CustomRespawnPractice);

class $modify (PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        PlayLayer::destroyPlayer(p0, p1);

        if (!p0)
            return;

        if (CustomRespawnTime::get()->getRealEnabled() || InstantRestart::get()->getRealEnabled())
        {
            if (auto action = getActionByTag(0x10))
            {
                this->stopActionByTag(0x10);

                float time = 0;

                if (!InstantRestart::get()->getRealEnabled() && CustomRespawnTime::get()->getRealEnabled())
                {
                    time = m_isPracticeMode ? CustomRespawnPractice::get()->getStringFloat() : CustomRespawnNormal::get()->getStringFloat();
                }

                auto act = CCSequence::create(CCDelayTime::create(time), CCCallFunc::create(this, callfunc_selector(PlayLayer::delayedResetLevel)), nullptr);
                act->setTag(0x10);

                this->runAction(act);
            }
        }
    }
};
// custom-respawn-time