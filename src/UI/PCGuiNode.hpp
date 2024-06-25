#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "../Client/Client.h"
#include "../Utils/CCBlurLayer.hpp"

using namespace geode::prelude;

class PCGuiNode : public CCNode
{
    private:
        static inline PCGuiNode* instance = nullptr;
    
    public:
        int v = 0;
        bool drawScene = false;
        CCBlurLayer* blur;
        CCClippingNode* clip;

        static PCGuiNode* create();
        static PCGuiNode* get();

        virtual void visit();
        virtual void draw();
        virtual void update(float delta);

        ~PCGuiNode();
};