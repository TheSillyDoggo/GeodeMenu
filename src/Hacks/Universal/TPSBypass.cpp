#include "../../Client/InputModule.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class TPSBypassEnabled : public Module
{
    public:
        MODULE_SETUP(TPSBypassEnabled)
        {
            setID("tps-bypass");
            setCategory("Universal");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
            setPriority(5);

            #ifdef GEODE_IS_ANDROID32
            setDisabled(true);
            setDisabledMessage("TPS Bypass is disabled on android32 due to breaking gameplay");
            #endif
        }
};

class TPSBypassValue : public InputModule
{
    public:
        MODULE_SETUP(TPSBypassValue)
        {
            setID("tps-bypass/value");

            setDefaultString("240");
            setPlaceholderString("TPS");
            setHint("tps");

            setStringFilter("1234567890.");
            setMaxCharCount(8);
        }

        double getTPSDelta()
        {
            return 1.0f / getStringFloat();
        }

        int getDeltaMod()
        {
            return std::ceil(getStringFloat() / (1.0f / CCDirector::get()->getDeltaTime()));
        }
};

SUBMIT_HACK(TPSBypassEnabled)
SUBMIT_OPTION(TPSBypassEnabled, TPSBypassValue)

#ifndef GEODE_IS_ANDROID32

class $modify (GJBaseGameLayer)
{
    double getModifiedDelta(float dt)
    {
        if (!TPSBypassEnabled::get()->getRealEnabled() || m_isEditor)
            return GJBaseGameLayer::getModifiedDelta(dt);

        double extraDelta = 0;
        double deltaRet = 0;

        if (m_resumeTimer < 1)
        {
            extraDelta = dt / TPSBypassValue::get()->getDeltaMod() + m_extraDelta;
        }
        else
        {
            m_resumeTimer += -1;
            extraDelta = m_extraDelta + 0.0f;
        }

        deltaRet = TPSBypassValue::get()->getTPSDelta() * std::min<float>(m_gameState.m_timeWarp, 1.0f);

        deltaRet = deltaRet * (int)(extraDelta / deltaRet);
        m_extraDelta = extraDelta - deltaRet;

        return deltaRet;
    }

    virtual void update(float dt)
    {
        if (TPSBypassEnabled::get()->getRealEnabled() && m_started && !m_isEditor)
        {
            int mod = TPSBypassValue::get()->getDeltaMod();
            for (size_t i = 0; i < mod; i++)
            {
                GJBaseGameLayer::update(dt);
            }
            
            return;
        }

        GJBaseGameLayer::update(dt);
    }
};

$execute
{
    // MOV R11D 0x1
    // https://defuse.ca/online-x86-assembler.htm
    // Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + 0x2322d2), { 0x41, 0xbb, 0x01, 0x00, 0x00, 0x00 });
    // Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + 0x2322ca), { 0x90, 0x90, 0x90, 0x90 });
};

#endif