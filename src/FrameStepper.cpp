#include "FrameStepper.h"

using namespace geode::prelude;

/*class $modify(UILayer)
{
    virtual void keyDown(enumKeyCodes key)
    {
        log::info("down");
        if (key == enumKeyCodes::KEY_Z)
            FrameStepper::keyDown = true;

        UILayer::keyDown(key);
    }

    virtual void keyUp(enumKeyCodes key)
    {
        log::info("up");
        FrameStepper::keyDown = false;
        UILayer::keyUp(key);
    }
};

class $modify(PlayLayer)
{
    virtual void update(float dt)
    {        
        PlayLayer::update(dt * (FrameStepper::keyDown ? 1.0f : 0.0f));
    }
};*/