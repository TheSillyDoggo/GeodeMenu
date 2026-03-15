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
            setPriority(7);
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

/*$execute
{
    // 140237a95 f3 0f 5f c8     MAXSS      XMM1,XMM0
    // fVar29 = (float)roundf((float)(((local_168 * 60.0) / (double)fVar37) * 4.0));
    // fVar37 = 1.0;
    // if (1.0 <= fVar29) {
    //   fVar37 = fVar29; // !!! disable this
    // }

    #if defined(GEODE_IS_WINDOWS) && GEODE_COMP_GD_VERSION == 22081
    #define TPSBYPASS_MIN_STEPS_PATCH_ADDR 0x237a95

    #elif defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION == 22081
    #define TPSBYPASS_MIN_STEPS_PATCH_ADDR 0x1fe748

    #elif defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION == 22074
    #define TPSBYPASS_MIN_STEPS_PATCH_ADDR 0x200c40
    #endif

    #ifdef TPSBYPASS_MIN_STEPS_PATCH_ADDR

    if (Loader::get()->isPatchless())
    {
        GEODE_MOD_STATIC_PATCH(TPSBYPASS_MIN_STEPS_PATCH_ADDR, {0x90, 0x90, 0x90, 0x90});
    }
    else
    {
        (void)Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + TPSBYPASS_MIN_STEPS_PATCH_ADDR), {0x90, 0x90, 0x90, 0x90});
    }

    #endif
};*/