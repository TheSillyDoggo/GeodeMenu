#include "../../Client/Module.hpp"
#include "../../Client/InputModule.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class Autoclicker : public Module
{
    public:
        MODULE_SETUP(Autoclicker)
        {
            setID("autoclicker");
            setCategory("Level");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

class AutoclickerP1 : public Module
{
    public:
        MODULE_SETUP(AutoclickerP1)
        {
            setID("autoclicker/player1");
            setPriority(1);
            setDefaultEnabled(true);
        }
};

class AutoclickerP2 : public Module
{
    public:
        MODULE_SETUP(AutoclickerP2)
        {
            setID("autoclicker/player2");
            setPriority(2);
            setDefaultEnabled(true);
        }
};

class AutoclickerP1Swift : public Module
{
    public:
        MODULE_SETUP(AutoclickerP1Swift)
        {
            setID("autoclicker/player1/swift");
            setPriority(2);
        }
};

class AutoclickerP2Swift : public Module
{
    public:
        MODULE_SETUP(AutoclickerP2Swift)
        {
            setID("autoclicker/player2/swift");
            setPriority(2);
        }
};

class AutoclickerP1Interval : public InputModule
{
    public:
        MODULE_SETUP(AutoclickerP1Interval)
        {
            setID("autoclicker/player1/interval");
            setPriority(1);

            setDefaultString("10");
            setPlaceholderString("CPS");
            setHint("cps");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }

        float getInterval()
        {
            if (getStringFloat() > 0)
            {
                float v = 1.0f / getStringFloat();

                if (!AutoclickerP1Swift::get()->getRealEnabled())
                    v /= 2;

                return v;
            }

            return 999999;
        }
};

class AutoclickerP2Interval : public InputModule
{
    public:
        MODULE_SETUP(AutoclickerP2Interval)
        {
            setID("autoclicker/player2/interval");
            setPriority(1);

            setDefaultString("10");
            setPlaceholderString("CPS");
            setHint("cps");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }

        float getInterval()
        {
            if (getStringFloat() > 0)
            {
                float v = 1.0f / getStringFloat();

                if (!AutoclickerP2Swift::get()->getRealEnabled())
                    v /= 2;

                return v;
            }

            return 999999;
        }
};

SUBMIT_HACK(Autoclicker);
SUBMIT_OPTION(Autoclicker, AutoclickerP1);
SUBMIT_OPTION(Autoclicker, AutoclickerP2);
SUBMIT_OPTION(AutoclickerP1, AutoclickerP1Interval);
SUBMIT_OPTION(AutoclickerP2, AutoclickerP2Interval);
SUBMIT_OPTION(AutoclickerP1, AutoclickerP1Swift);
SUBMIT_OPTION(AutoclickerP2, AutoclickerP2Swift);

class $modify (AutoclickerBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        double timeP1 = 0;
        double timeP2 = 0;
        bool p1State = false;
        bool p2State = false;
    };

    void handleClick(bool swift, bool down, bool player2)
    {
        if (swift)
        {
            this->GJBaseGameLayer::handleButton(true, (int)PlayerButton::Jump, !player2);
            this->GJBaseGameLayer::handleButton(false, (int)PlayerButton::Jump, !player2);
        }
        else
        {
            this->GJBaseGameLayer::handleButton(down, (int)PlayerButton::Jump, !player2);
        }
    }

    void processCommands(float dt, bool isHalfTick, bool isLastTick)
    {
        if (!Autoclicker::get()->getRealEnabled())
            return GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);

        auto fields = m_fields.self();
        float p1Interval = AutoclickerP1Interval::get()->getInterval();
        float p2Interval = AutoclickerP2Interval::get()->getInterval();

        if (AutoclickerP1::get()->getRealEnabled())
        {
            fields->timeP1 += dt;
            while (fields->timeP1 > p1Interval)
            {
                fields->p1State = !fields->p1State;
                fields->timeP1 -= p1Interval;

                handleClick(AutoclickerP1Swift::get()->getRealEnabled(), fields->p1State, false);
            }
        }

        if (AutoclickerP2::get()->getRealEnabled())
        {
            fields->timeP2 += dt;
            while (fields->timeP2 > p2Interval)
            {
                fields->p2State = !fields->p2State;
                fields->timeP2 -= p2Interval;

                handleClick(AutoclickerP2Swift::get()->getRealEnabled(), fields->p2State, false);
            }
        }

        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }

    void resetLevelVariables()
    {
        GJBaseGameLayer::resetLevelVariables();

        m_fields->timeP1 = 0;
        m_fields->timeP2 = 0;
        m_fields->p1State = false;
        m_fields->p2State = false;
    }
};