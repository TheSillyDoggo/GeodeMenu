#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>

class $modify (NotificationEGLView, cocos2d::CCEGLView)
{
    virtual void swapBuffers();
};