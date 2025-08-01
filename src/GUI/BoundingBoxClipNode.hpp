#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BoundingBoxClipNode : public CCNode
{
    public:
        CCNode* node;

        virtual bool init();
        virtual void visit();

        CREATE_FUNC(BoundingBoxClipNode)
};