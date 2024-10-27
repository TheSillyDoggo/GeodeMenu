#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class UnspeedhackedAction : public CCAction
{
    public:
        Ref<CCAction> action;

        bool init(CCAction* action);

        virtual void step(float dt);
        virtual void stop();
        virtual bool isDone();
        virtual void startWithTarget(CCNode *pTarget);

        static UnspeedhackedAction* create(CCAction* action);
};