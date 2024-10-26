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

        std::vector<float> cps1;
        std::vector<float> cps2;

        int clicks1;
        int clicks2;
        float lastPercentage;
    
    public:
        UILayer* uiLayer;

        bool init(UILayer* uiLayer);

        void triggerEvent(LabelEventType type);

        void setLastPercentage(float v);
        float getLastPercentage();

        void incrementAttempts();
        int getAttempts();

        void increateCPS(bool player2);
        void resetCPS();
        int getCPS(bool player2);
        int getTotalCPS();

        int getClicks(bool player2);
        int getTotalClicks();

        float getFPS();

        CCNode* createNodeForAnchor(LabelAnchor anchor);
        CCNode* nodeForAnchor(LabelAnchor anchor);

        virtual void update(float dt);

        void updateLabels();
        void updateAnchors();

        ~LabelLayer();

        static LabelLayer* create(UILayer* uiLayer);
        static LabelLayer* get();
};