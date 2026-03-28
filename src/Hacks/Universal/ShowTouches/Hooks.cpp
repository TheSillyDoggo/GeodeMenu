#include "Colours.hpp"
#include <Geode/modify/CCTouchDispatcher.hpp>
#include <Geode/modify/CCEGLViewProtocol.hpp>
#include "ShowTouchLayer.hpp"

using namespace geode::prelude;

class ShowTouches : public Module
{
    public:
        MODULE_SETUP(ShowTouches)
        {
            setName("Show Touches");
            setID("show-touches");
            setCategory("Universal");
            setDescription("Shows a circle on the screen whenever you touch");
        }
};

SUBMIT_HACK(ShowTouches)
SUBMIT_OPTION(ShowTouches, CircleColour)
SUBMIT_OPTION(ShowTouches, TrailColour)
SUBMIT_OPTION(ShowTouches, CircleScale)
SUBMIT_OPTION(ShowTouches, TrailEnabled)
SUBMIT_OPTION(ShowTouches, ShowTouchDuration)

#define TOUCHED_MACRO($func) \
for (size_t i = 0; i < num; i++) \
{ \
    int id = ids[i]; \
    float x = xs[i]; \
    float y = ys[i]; \
    qolmod::ShowTouchLayer::get()->$func(id, ccp( \
        (x - m_obViewPortRect.origin.x) / m_fScaleX, \
        (m_obViewPortRect.size.height - y - m_obViewPortRect.origin.y) / m_fScaleY \
    )); \
}

#if GEODE_COMP_GD_VERSION >= 22081
#define TOUCH_PARAMS int num, int ids[], float xs[], float ys[], double timestamp
#define TOUCH_PARAMS2 num, ids, xs, ys, timestamp
#else
#define TOUCH_PARAMS int num, int ids[], float xs[], float ys[]
#define TOUCH_PARAMS2 num, ids, xs, ys
#endif

class $modify (TouchedEGLViewProtocol, CCEGLViewProtocol)
{
    virtual void handleTouchesBegin(TOUCH_PARAMS)
    {
        CCEGLViewProtocol::handleTouchesBegin(TOUCH_PARAMS2);
        TOUCHED_MACRO(touchBegan);
    }

    virtual void handleTouchesMove(TOUCH_PARAMS)
    {
        CCEGLViewProtocol::handleTouchesMove(TOUCH_PARAMS2);
        TOUCHED_MACRO(touchMoved);
    }
    
    virtual void handleTouchesEnd(TOUCH_PARAMS)
    {
        CCEGLViewProtocol::handleTouchesEnd(TOUCH_PARAMS2);
        TOUCHED_MACRO(touchEnded);
    }

    virtual void handleTouchesCancel(TOUCH_PARAMS)
    {
        CCEGLViewProtocol::handleTouchesCancel(TOUCH_PARAMS2);
        TOUCHED_MACRO(touchCancelled);
    }
};