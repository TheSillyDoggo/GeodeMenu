#include "../../Client/Module.hpp"
#include <Geode/modify/CCCircleWave.hpp>
#include <Geode/modify/GameObject.hpp>

using namespace geode::prelude;

class NoCirclesClassic : public Module
{
    public:
        MODULE_SETUP(NoCirclesClassic)
        {
            setID("no-circles");
            setCategory("Cosmetic");
        }
};

class NoCirclesAdv : public Module
{
    public:
        MODULE_SETUP(NoCirclesAdv)
        {
            setID("no-circles-adv");
            setCategory("Cosmetic");
        }
};

class NoOrbTouchCircle : public Module
{
    public:
        MODULE_SETUP(NoOrbTouchCircle)
        {
            setID("no-circles-adv/orb-touch");
            setDefaultEnabled(true);
            setPriority(1);
        }
};

class NoOrbClickCircle : public Module
{
    public:
        MODULE_SETUP(NoOrbClickCircle)
        {
            setID("no-circles-adv/orb-click");
            setDefaultEnabled(true);
            setPriority(2);
        }
};

class NoPadTouchCircle : public Module
{
    public:
        MODULE_SETUP(NoPadTouchCircle)
        {
            setID("no-circles-adv/pad-touch");
            setDefaultEnabled(true);
            setPriority(3);
        }
};

class NoGravityTouchCircle : public Module
{
    public:
        MODULE_SETUP(NoGravityTouchCircle)
        {
            setID("no-circles-adv/gravity-portal-touch");
            setDefaultEnabled(true);
            setPriority(4);
        }
};

class NoGamemodePortalCircle : public Module
{
    public:
        MODULE_SETUP(NoGamemodePortalCircle)
        {
            setID("no-circles-adv/gamemode-portal-touch");
            setDefaultEnabled(true);
            setPriority(5);
        }
};

class NoSizePortalCircle : public Module
{
    public:
        MODULE_SETUP(NoSizePortalCircle)
        {
            setID("no-circles-adv/size-portal-touch");
            setDefaultEnabled(true);
            setPriority(6);
        }
};

SUBMIT_HACK(NoCirclesClassic);
SUBMIT_HACK(NoCirclesAdv);
SUBMIT_OPTION(NoCirclesAdv, NoOrbTouchCircle);
SUBMIT_OPTION(NoCirclesAdv, NoOrbClickCircle);
SUBMIT_OPTION(NoCirclesAdv, NoPadTouchCircle);
SUBMIT_OPTION(NoCirclesAdv, NoGravityTouchCircle);
SUBMIT_OPTION(NoCirclesAdv, NoGamemodePortalCircle);
SUBMIT_OPTION(NoCirclesAdv, NoSizePortalCircle);

class $modify (CCCircleWave)
{
    struct Fields
    {
        float startRadius;
        float endRadius;
        float duration;
        bool fadeIn;
        bool easeOut;
    };

    bool init(float startRadius, float endRadius, float duration, bool fadeIn, bool easeOut)
    {
        m_fields->startRadius = startRadius;
        m_fields->endRadius = endRadius;
        m_fields->duration = duration;
        m_fields->fadeIn = fadeIn;
        m_fields->easeOut = easeOut;

        return CCCircleWave::init(startRadius, endRadius, duration, fadeIn, easeOut);
    }

    virtual void draw()
    {
        if (getUserFlag("allow-circle"_spr))
            return CCCircleWave::draw();

        if (PlayLayer::get())
        {
            if (NoCirclesClassic::get()->getRealEnabled())
                return;

            auto fields = m_fields.self();

            if (NoCirclesAdv::get()->getRealEnabled())
            {
                if (NoOrbTouchCircle::get()->getRealEnabled())
                {
                    if (m_circleMode == CircleMode::Outline)
                    {
                        if (fields->startRadius == 5 && fields->endRadius == 55 && fields->duration == 0.25f)
                            return;
                    }
                }

                if (m_circleMode == CircleMode::Filled)
                {
                    if (NoOrbClickCircle::get()->getRealEnabled())
                    {
                        if (fields->startRadius == 35 && fields->endRadius == 5 && fields->duration == 0.35f)
                            return;

                        if (fields->startRadius == 42 && fields->endRadius == 5 && fields->duration == 0.35f)
                            return;
                    }

                    if (NoPadTouchCircle::get()->getRealEnabled())
                    {
                        if (fields->startRadius == 10 && fields->endRadius == 40 && fields->duration == 0.25f)
                            return;

                        if (fields->startRadius == 10 && fields->endRadius == 45 && fields->duration == 0.25f)
                            return;

                        if (fields->startRadius == 12 && fields->endRadius == 40 && fields->duration == 0.25f)
                            return;
                    }

                    if (NoGravityTouchCircle::get()->getRealEnabled())
                    {
                        if (fields->startRadius == 45 && fields->endRadius == 5 && fields->duration == 0.3f)
                            return;
                    }

                    if (NoGamemodePortalCircle::get()->getRealEnabled())
                    {
                        if (fields->startRadius == 50 && fields->endRadius == 5 && fields->duration == 0.3f)
                            return;
                    }

                    if (NoSizePortalCircle::get()->getRealEnabled())
                    {
                        if (fields->startRadius == 50 && fields->endRadius == 2 && fields->duration == 0.25f)
                            return;

                        if (fields->startRadius == 50 && fields->endRadius == 5 && fields->duration == 0.25f)
                            return;

                        // dual end
                        if (fields->startRadius == 50 && fields->endRadius == 5 && fields->duration == 0.3f)
                            return;

                        if (fields->startRadius == 45 && fields->endRadius == 5 && fields->duration == 0.3f)
                            return;
                    }
                }
            }
        }

        CCCircleWave::draw();
    }
};