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

class $modify (TouchedEGLViewProtocol, CCEGLViewProtocol)
{
    virtual void handleTouchesBegin(int num, int ids[], float xs[], float ys[], double timestamp)
    {
        CCEGLViewProtocol::handleTouchesBegin(num, ids, xs, ys, timestamp);
        TOUCHED_MACRO(touchBegan);
    }

    virtual void handleTouchesMove(int num, int ids[], float xs[], float ys[], double timestamp)
    {
        CCEGLViewProtocol::handleTouchesMove(num, ids, xs, ys, timestamp);
        TOUCHED_MACRO(touchMoved);
    }
    
    virtual void handleTouchesEnd(int num, int ids[], float xs[], float ys[], double timestamp)
    {
        CCEGLViewProtocol::handleTouchesEnd(num, ids, xs, ys, timestamp);
        TOUCHED_MACRO(touchEnded);
    }

    virtual void handleTouchesCancel(int num, int ids[], float xs[], float ys[], double timestamp)
    {
        CCEGLViewProtocol::handleTouchesCancel(num, ids, xs, ys, timestamp);
        TOUCHED_MACRO(touchCancelled);
    }
};