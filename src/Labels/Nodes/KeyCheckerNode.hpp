#pragma once

#include "LabelNode.hpp"
#include <rift.hpp>
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

class KeyCheckerNode : public LabelNode
{
    protected:
        CCLayerColor* layer = nullptr;
        AdvLabelBMFont* labelUpArrow = nullptr;

    public:
        CREATE_FUNC(KeyCheckerNode);

        virtual void setup();
        virtual void labelConfigUpdated();
        virtual void update(float dt);
};