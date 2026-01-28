#pragma once

#include <Geode/Geode.hpp>
#include "../LabelConfig.hpp"

using namespace geode::prelude;

class LabelNode : public CCNode
{
    protected:
        LabelConfig config;

        virtual bool init();

    public:
        static LabelNode* createForType(LabelType type);

        void setLabelConfig(LabelConfig config);
        const LabelConfig& getLabelConfig();

        void updateGeneral(float dt);
        virtual bool isActionActive();
        bool isVisible();

        virtual void setup();
        virtual void labelConfigUpdated();
        virtual void update(float dt);
        virtual void visit(void);
        virtual void onEventTriggered(LabelEventType type);
};