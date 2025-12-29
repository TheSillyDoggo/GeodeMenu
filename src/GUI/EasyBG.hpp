#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class EasyBG : public CCNode
{
    protected:
        CCScale9Sprite* bg = nullptr;
        CCNode* targetting = nullptr;
        CCSize targettingOffset = CCPointZero;

    public:
        CREATE_FUNC(EasyBG)

        void setTargettingNode(CCNode* target);
        void setTargettingOffset(CCSize offset);

        virtual bool init();
        virtual void visit();
};