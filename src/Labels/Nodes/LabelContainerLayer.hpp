#pragma once

#include "LabelNode.hpp"
#include "../LabelManager.hpp"

class LabelContainerLayer : public CCLayer
{
    protected:
        std::vector<LabelNode*> nodes = {};
        std::unordered_map<LabelAnchor, CCNode*> anchors = {};

    public:
        CREATE_FUNC(LabelContainerLayer);

        void addAnchorNode(LabelAnchor anchor);
        void updateConfigs();
        void sortNodeChildren(CCNode* node);

        virtual bool init();
        virtual void update(float dt);
};