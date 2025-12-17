#include "Colours.hpp"
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "TouchTrail.hpp"

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


class $modify (CCTouchDispatcher)
{
    void touches(CCSet* touches, CCEvent* event, unsigned int type)
    {
        if (!CCScene::get())
            return CCTouchDispatcher::touches(touches, event, type);;

        if (CCScene::get()->getChildByType<LoadingLayer>(0))
            return CCTouchDispatcher::touches(touches, event, type);;

        if (auto touch = as<CCTouch*>(touches->anyObject()))
        {
            if (type == CCTOUCHBEGAN && ShowTouches::get()->getRealEnabled())
            {
                CCScene::get()->addChild(CCTouchTrail::create(touch), CCScene::get()->getHighestChildZ() + 1);
            }

            if (type >= CCTOUCHENDED)
            {
                CCTouchTrail::remove(touch);
            }
        }

        CCTouchDispatcher::touches(touches, event, type);
    }
};