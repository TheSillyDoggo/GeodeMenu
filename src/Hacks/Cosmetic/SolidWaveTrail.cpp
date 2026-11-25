#include "../../Client/Module.hpp"
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/CCDrawNode.hpp>

using namespace geode::prelude;

class SolidWaveTrail : public Module
{
    public:
        MODULE_SETUP(SolidWaveTrail)
        {
            setID("solid-wave");
            setCategory("Cosmetic");
        }
};

class SolidWaveTrailPreserveCenter : public Module
{
    public:
        MODULE_SETUP(SolidWaveTrailPreserveCenter)
        {
            setID("solid-wave/preserve-center");
        }
};

SUBMIT_HACK(SolidWaveTrail);
SUBMIT_OPTION(SolidWaveTrail, SolidWaveTrailPreserveCenter);

class $modify (HardStreak)
{
    void appendQuad(cocos2d::CCPoint const* vertices, cocos2d::ccColor4F const& color)
    {
        if (color.a != getOpacity() && !SolidWaveTrailPreserveCenter::get()->getRealEnabled())
            return;

        HardStreak::appendQuad(vertices, color);
    }

    void updateStroke(float dt)
    {
        if (SolidWaveTrail::get()->getRealEnabled())
        {
            setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        }

        HardStreak::updateStroke(dt);
    }
};