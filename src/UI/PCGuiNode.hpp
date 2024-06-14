/*#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCDirector.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class PCGuiNode : public CCNode
{
    private:
        static inline PCGuiNode* instance = nullptr;
    
    public:
        int v = 0;
        bool drawScene = false;

        static PCGuiNode* create();
        static PCGuiNode* get();

        virtual void visit();
        virtual void draw();

        ~PCGuiNode();
};*/