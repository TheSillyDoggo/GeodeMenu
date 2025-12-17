#pragma once

#include "LabelConfig.hpp"
#include <chrono>

class LabelManager
{
    friend class SpeedhackScheduler;

    protected:
        std::vector<LabelConfig> configs = {};
        std::chrono::steady_clock::time_point sessionBegin;
        float fps;
        float _updateInterval = 0.5f;
        float _timeLeft = _updateInterval;
        float _accum = 0;
        int _frames = 0;

        void update(float dt);

    public:
        static LabelManager* get();

        const std::vector<LabelConfig>& getConfigs();

        cocos2d::CCPoint anchorToPoint(LabelAnchor anchor);

        float getSessionDuration();
        int getAttemptCount();
        float getFPS();

        void load();
        void save();
};