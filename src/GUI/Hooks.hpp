#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>

using namespace geode::prelude;

class $modify (BallEGLView, CCEGLView)
{
    virtual void swapBuffers();
};

class $modify (BallGameManager, GameManager)
{
    void reloadAllStep5();
};

class $modify (BallTouchDispatcher, CCTouchDispatcher)
{
    void touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex);
};