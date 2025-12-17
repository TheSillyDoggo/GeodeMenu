#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RealtimeAction : public CCAction
{
    public:
        Ref<CCAction> action;

        bool init(CCAction* action);

        virtual void step(float dt);
        virtual void stop();
        virtual bool isDone();
        virtual void startWithTarget(CCNode *pTarget);

        static RealtimeAction* create(CCAction* action);
};