#pragma once
#include "RangeManager.hpp"

namespace qolmod
{
    struct RangeActivationData
    {
        ModRangeConfig* m_globalConf;
        ModRangeConfig* m_levelConf;
        bool m_inRange;
        bool m_rangeEnable;
        bool m_lastFrameWasInRange;
        bool m_lastFrameWasEnabled;
    };

    class RangeActivationController
    {
        protected:
            std::unordered_map<Module*, RangeActivationData> m_data = {};

            void notifyRange(Module* module, float percent);

        public:
            static RangeActivationController* get();

            bool isInRange(Module* module, bool& enabledOut);

            void updateForPercent(float percent, GJBaseGameLayer* layer);
            void reset(GJBaseGameLayer* layer); // call on open level
    };
};