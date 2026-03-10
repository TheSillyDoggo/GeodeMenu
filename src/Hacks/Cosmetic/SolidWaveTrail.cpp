#include "../../Client/Module.hpp"

using namespace geode::prelude;

class SolidWaveTrail : public Module
{
    public:
        MODULE_SETUP(SolidWaveTrail)
        {
            setName("Solid Wave Trail");
            setID("solid-wave");
            setCategory("Cosmetic");
            setDescription("Makes the wave trail a solid colour");

            #if GEODE_COMP_GD_VERSION <= 22074
            setDisabledMessage("Unsupported in GD 2.2074");
            setDisabled(true);
            #endif
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

#if GEODE_COMP_GD_VERSION >= 22081

#include <Geode/modify/CCDrawNode.hpp>

class $modify (CCDrawNode)
{
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor, cocos2d::BorderAlignment alignment)
    {
        ccColor4F fill = fillColor;

        if (SolidWaveTrail::get()->getRealEnabled())
        {
            if (typeinfo_cast<HardStreak*>(this))
            {
                if (!SolidWaveTrailPreserveCenter::get()->getRealEnabled())
                    fill = ccc4f(getColor().r / 255.0f, getColor().g / 255.0f, getColor().b / 255.0f, getOpacity() / 255.0f);
                else
                    fill.a = getOpacity() / 255.0f;
                
                setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
            }
        }

        return CCDrawNode::drawPolygon(verts, count, fill, borderWidth, borderColor, alignment);
    }
};

#endif