#ifdef STATUS_TEXTS

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"
#include "LabelNode.hpp"
#include "../Hacks/Noclip.cpp"

using namespace geode::prelude;

class $modify (LabelPlayLayer, PlayLayer)
{
    struct Fields {
        int attemptCount = 0;
    };

    bool init(GJGameLevel* p0, bool p1, bool p2);
    void resetLevel();
};

class StatusNode : public CCNode
{
    private:
        static inline StatusNode* instance = nullptr;
        float _updateInterval = 0.5f;
        float _timeLeft = _updateInterval;
        float _accum;
        int _frames;
    
    public:
        int fps;
        std::vector<LabelNode*> labels;

        static StatusNode* create();
        static StatusNode* get();

        ~StatusNode()
        {
            instance = nullptr;
        }

        LabelPlayLayer* attPL = nullptr;

        CCMenu* topLeft = nullptr;
        CCMenu* topRight = nullptr;
        CCMenu* bottomLeft = nullptr;
        CCMenu* bottomRight = nullptr;

        static inline Window* window = nullptr;

        static inline bool hidden = false;

        std::vector<float> cps;

        std::string formatTime(float time) {
            // Convert float time to milliseconds
            std::chrono::milliseconds duration(static_cast<long long>(time * 1000));

            // Extract hours, minutes, and seconds
            auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
            duration -= hours;
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
            duration -= minutes;
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

            // Format the time into hh:mm:ss
            std::ostringstream formattedTime;
            formattedTime << std::setfill('0') << std::setw(2) << hours.count() << ":"
                        << std::setfill('0') << std::setw(2) << minutes.count() << ":"
                        << std::setfill('0') << std::setw(2) << seconds.count();
            
            return formattedTime.str();
        }


        bool init();

        void updateVis();

        static void postSetup(Window* wnd);

        void reorderSides();
        void reorderPosition();

        virtual void update(float dt);
        void updateCPS(float dt);
};

#endif