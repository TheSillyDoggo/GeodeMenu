#pragma once

#include "LabelNode.hpp"
#include <rift.hpp>
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

class TextLabelNode : public LabelNode
{
    protected:
        AdvLabelBMFont* label = nullptr;
        rift::Script* script = nullptr;

        ~TextLabelNode();

    public:
        CREATE_FUNC(TextLabelNode);

        ccColor3B getDesiredColour();
        void updateVariables();

        virtual void setup();
        virtual void labelConfigUpdated();
        virtual void onEventTriggered(LabelEventType type);
        virtual void update(float dt);
        virtual bool isActionActive();
};