#include "Touch.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLViewProtocol.hpp>
#include <../Hacks/Universal/ShowTouches/ShowTouchLayer.hpp>
#include <FloatingButton/FloatingUIManager.hpp>

using namespace geode::prelude;

qolmod::Touch::Touch(int id, cocos2d::CCPoint point)
{
    fakeTouch = new CCTouch();
    startLocation = point;
    location = point;

    updateFakeTouch();
}

qolmod::Touch::~Touch()
{
    delete fakeTouch;
}

void qolmod::Touch::updateFakeTouch()
{
    fakeTouch->setTouchInfo(id, location.x, CCDirector::get()->getWinSize().height - location.y);
}
    

#define TOUCHED_MACRO($func) \
for (size_t i = 0; i < num; i++) \
{ \
    qolmod::ShowTouchLayer::get()->$func(touches[ids[i]]); \
}

#if GEODE_COMP_GD_VERSION >= 22081
#define TOUCH_PARAMS int num, int ids[], float xs[], float ys[], double timestamp
#define TOUCH_PARAMS2 num, ids, xs, ys, timestamp
#define TOUCH_PARAMS3 ids2.size(), id, x, y, timestamp
#else
#define TOUCH_PARAMS int num, int ids[], float xs[], float ys[]
#define TOUCH_PARAMS2 num, ids, xs, ys
#define TOUCH_PARAMS3 ids2.size(), id, x, y
#endif

#define LOOP_MACRO($code) \
std::vector<int> ids2 = {}; \
std::vector<float> xs2 = {}; \
std::vector<float> ys2 = {}; \
for (size_t i = 0; i < num; i++) \
{ \
    $code \
    ids2.push_back(ids[i]); \
    xs2.push_back(xs[i]); \
    ys2.push_back(ys[i]); \
} \
int id[ids2.size()];std::copy(ids2.begin(), ids2.end(), id); \
float x[ids2.size()];std::copy(xs2.begin(), xs2.end(), x); \
float y[ids2.size()];std::copy(ys2.begin(), ys2.end(), y); \

std::unordered_map<int, qolmod::Touch*> touches = {};

class $modify (TouchedEGLViewProtocol, CCEGLViewProtocol)
{
    virtual void handleTouchesBegin(TOUCH_PARAMS)
    {
        for (size_t i = 0; i < num; i++)
        {
            if (touches.contains(ids[i]))
                delete touches[ids[i]];

            touches[ids[i]] = new qolmod::Touch(ids[i], screenToCocos(xs[i], ys[i]));
        }

        TOUCHED_MACRO(touchBegan);

        LOOP_MACRO(
            if (FloatingUIManager::get()->touchBegan(touches[ids[i]]))
                continue;
        );

        CCEGLViewProtocol::handleTouchesBegin(TOUCH_PARAMS3);
    }

    virtual void handleTouchesMove(TOUCH_PARAMS)
    {
        for (size_t i = 0; i < num; i++)
        {
            touches[ids[i]]->location = screenToCocos(xs[i], ys[i]);
            touches[ids[i]]->updateFakeTouch();
        }

        TOUCHED_MACRO(touchMoved);

        LOOP_MACRO(
            if (FloatingUIManager::get()->touchMoved(touches[ids[i]]))
                continue;
        );
        
        CCEGLViewProtocol::handleTouchesMove(TOUCH_PARAMS3);
    }
    
    virtual void handleTouchesEnd(TOUCH_PARAMS)
    {
        for (size_t i = 0; i < num; i++)
        {
            touches[ids[i]]->location = screenToCocos(xs[i], ys[i]);
            touches[ids[i]]->updateFakeTouch();
        }

        TOUCHED_MACRO(touchEnded);

        LOOP_MACRO(
            if (FloatingUIManager::get()->touchEnded(touches[ids[i]]))
                continue;
        );
        CCEGLViewProtocol::handleTouchesEnd(TOUCH_PARAMS3);
    }

    virtual void handleTouchesCancel(TOUCH_PARAMS)
    {
        for (size_t i = 0; i < num; i++)
        {
            touches[ids[i]]->location = screenToCocos(xs[i], ys[i]);
            touches[ids[i]]->updateFakeTouch();
        }

        TOUCHED_MACRO(touchCancelled);

        LOOP_MACRO(
            if (FloatingUIManager::get()->touchCancelled(touches[ids[i]]))
                continue;
        );
        CCEGLViewProtocol::handleTouchesCancel(TOUCH_PARAMS3);
    }

    cocos2d::CCPoint screenToCocos(float x, float y)
    {
        return ccp(
            (x - m_obViewPortRect.origin.x) / m_fScaleX,
            (m_obViewPortRect.size.height - y - m_obViewPortRect.origin.y) / m_fScaleY
        );
    }
};