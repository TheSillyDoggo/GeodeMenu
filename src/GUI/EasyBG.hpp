#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class EasyBG : public CCNode
{
    protected:
        NineSlice* bg = nullptr;
        CCNode* targetting = nullptr;
        CCSize targettingOffset = CCPointZero;

    public:
        CREATE_FUNC(EasyBG)

        void setTargettingNode(CCNode* target);
        void setTargettingOffset(CCSize offset);

        NineSlice* getBG();

        virtual bool init();
        virtual void visit();
};