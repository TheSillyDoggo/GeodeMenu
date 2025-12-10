#include "../../Client/Module.hpp"
#include "../../Client/FloatSliderModule.hpp"
#include "../../Client/ColourModule.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class Noclip : public Module
{
    public:
        MODULE_SETUP(Noclip)
        {
            setName("Noclip");
            setID("noclip");
            setCategory("Level");
            setDescription("Prevents the player from dying");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
            setPriority(1);
        }
};

class NoclipTintOnDeath : public Module
{
    public:
        MODULE_SETUP(NoclipTintOnDeath)
        {
            setName("Tint on death");
            setID("noclip/tint-on-death");
            setDescription("Tints the screen when you die");
            setPriority(1);
        }
};

class NoclipTintOpacity : public FloatSliderModule
{
    public:
        MODULE_SETUP(NoclipTintOpacity)
        {
            setName("Tint Opacity:");
            setID("noclip/tint-opacity");
            setDefaultValue(0.25f);
            setSnapValues({ 0.25f });
            setRange(0.05f, 1.0f);
            setPriority(2);
        }
};

class NoclipTintColour : public ColourModule
{
    public:
        MODULE_SETUP(NoclipTintColour)
        {
            setName("Tint Colour:");
            setID("noclip/tint-colour");
            setDefaultConfig({ ccc3(255, 0, 0)} );
            setPriority(3);
        }
};

SUBMIT_HACK(Noclip);
SUBMIT_OPTION(Noclip, NoclipTintOnDeath);
SUBMIT_OPTION(Noclip, NoclipTintOpacity);
SUBMIT_OPTION(Noclip, NoclipTintColour);

class $modify (PlayLayer)
{
    struct Fields
    {
        CCLayerColor* tintOverlay;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        m_fields->tintOverlay = CCLayerColor::create(ccc4(0, 0, 0, 0));
        m_fields->tintOverlay->setID("noclip-tint-overlay"_spr);
        m_fields->tintOverlay->setZOrder(1);
        
        this->addChild(m_fields->tintOverlay);
        return true;
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        if (p1 == m_anticheatSpike)
            return PlayLayer::destroyPlayer(p0, p1);

        if (!Noclip::get()->getRealEnabled())
            return PlayLayer::destroyPlayer(p0, p1);

        if (NoclipTintOnDeath::get()->getRealEnabled())
        {
            m_fields->tintOverlay->stopAllActions();
            m_fields->tintOverlay->setOpacity(NoclipTintOpacity::get()->getValue() * 255.0f);
            m_fields->tintOverlay->runAction(CCFadeTo::create(0.35f, 0));
        }
    }

    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        m_fields->tintOverlay->setColor(NoclipTintColour::get()->getColour());
    }
};