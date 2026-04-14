#pragma once

#include <Geode/Geode.hpp>
#include "../Hitboxes/HitboxNode.hpp"

namespace qolmod
{
    class TrajectoryNode : public HitboxNode
    {
        protected:
            static inline TrajectoryNode* instance = nullptr;
            PlayerObject* player = nullptr;
            bool simulating = false;
            float deltaIter = 0.5f;
            int iterCount = 240;

            ~TrajectoryNode();
            
            virtual bool shouldDrawTrail();

        public:
            CREATE_FUNC(TrajectoryNode);
            static TrajectoryNode* get();

            bool isSimulating();
            void simulate(PlayerObject* plr, bool held);

            float getDeltaIter();
            int getIterCount();

            void setDeltaIter(float delta);
            void setIterCount(int iters);

            virtual void redraw();
            virtual bool init();
    };
}