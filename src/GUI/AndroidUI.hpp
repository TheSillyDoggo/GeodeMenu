#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"

using namespace geode::prelude;

class AndroidUI : public geode::Popup<>
{
    protected:
        static inline AndroidUI* instance = nullptr;

        BackgroundSprite* bg = nullptr;

        ~AndroidUI();

    public:
        static AndroidUI* create();
        static AndroidUI* addToScene();
        static AndroidUI* get();

        void test(CCObject* sender);

        void populateModules();
        void close();
        virtual bool setup();
};