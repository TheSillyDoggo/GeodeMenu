#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>

using namespace geode::prelude;

class $modify (SpeedhackScheduler, CCScheduler)
{
    virtual void update(float dt);
};