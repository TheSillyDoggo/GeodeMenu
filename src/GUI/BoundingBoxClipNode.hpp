#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BoundingBoxClipNode : public CCNode
{
    private:
        CCNode* node;

    public:
        virtual bool init();
        virtual void visit();

        CREATE_FUNC(BoundingBoxClipNode)
};