#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify (BallGameManager, GameManager)
{
    void reloadAllStep5();
};

class $modify (BallTouchDispatcher, CCTouchDispatcher)
{
    void touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex);
};

class $modify (BallMenuLayer, MenuLayer)
{
    virtual bool init();
};