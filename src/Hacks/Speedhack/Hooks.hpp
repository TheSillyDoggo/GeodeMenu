#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify (CBFCheckMenuLayer, MenuLayer)
{
    virtual bool init();
};

class $modify (SpeedhackScheduler, CCScheduler)
{
    virtual void update(float dt);
};

class $modify (SpeedhackBaseGameLayer, GJBaseGameLayer)
{
    virtual void update(float dt);
};