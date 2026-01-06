#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>

using namespace geode::prelude;

class $modify (FloatingMenuLayer, MenuLayer)
{
    bool init();
};

class $modify (FloatingTouchDispatcher, CCTouchDispatcher)
{
    void touches(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent, unsigned int uIndex);
};