#pragma once

#include <Geode/Geode.hpp>
#include "BestRun.hpp"
#include "LabelCommon.hpp"
#include "LabelNode.hpp"
#include "../Client/Client.h"

using namespace geode::prelude;

class LabelLayer : public CCLayer
{
    private:
        float fps;
        float _updateInterval = 0.5f;
        float _timeLeft = _updateInterval;
        float _accum;
        int _frames;

        int attempts;
        std::map<LabelAnchor, CCNode*> nodes;
        std::vector<LabelNode*> labels;
        static inline LabelLayer* instance;
    
    public:
        UILayer* uiLayer;

        bool init(UILayer* uiLayer);

        void incrementAttempts();
        int getAttempts();

        float getFPS();
        int getCPS();

        CCNode* createNodeForAnchor(LabelAnchor anchor);
        CCNode* nodeForAnchor(LabelAnchor anchor);

        virtual void update(float dt);

        void updateAnchors();

        ~LabelLayer();

        static LabelLayer* create(UILayer* uiLayer);
        static LabelLayer* get();
};