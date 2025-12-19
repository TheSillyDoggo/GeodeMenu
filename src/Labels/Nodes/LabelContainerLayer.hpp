#pragma once

#include "LabelNode.hpp"
#include "../LabelManager.hpp"
#include "../../Hacks/Level/Noclip/Hooks.hpp"

class LabelContainerLayer : public CCLayer
{
    protected:
        static inline LabelContainerLayer* instance = nullptr;

        std::vector<LabelNode*> nodes = {};
        std::unordered_map<LabelAnchor, CCNode*> anchors = {};
        int totalClicks = 0;
        int p1Clicks = 0;
        int p2Clicks = 0;

        std::vector<float> totalCps;
        std::vector<float> p1Cps;
        std::vector<float> p2Cps;

        ~LabelContainerLayer();

    public:
        CREATE_FUNC(LabelContainerLayer);
        static LabelContainerLayer* get();

        void addAnchorNode(LabelAnchor anchor);
        void updateConfigs();
        void sortNodeChildren(CCNode* node);

        int getCPS(NoclipPlayerSelector selector);
        int getTotalClicks(NoclipPlayerSelector selector);

        void onEventTriggered(LabelEventType type);
        void onPlayerClicked(NoclipPlayerSelector selector);
        void onNewAttempt();

        virtual bool init();
        virtual void update(float dt);
        virtual void visit(void);
};