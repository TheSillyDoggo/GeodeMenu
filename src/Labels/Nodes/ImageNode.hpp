#pragma once

#include "LabelNode.hpp"

class ImageNode : public LabelNode
{
    protected:
        CCSprite* sprite = nullptr;

    public:
        CREATE_FUNC(ImageNode);

        virtual void labelConfigUpdated();
        virtual void update(float dt);
};