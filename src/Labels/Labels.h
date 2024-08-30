#ifdef STATUS_TEXTS

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "BestRun.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "../Client/Client.h"

using namespace geode::prelude;

class NoclipBaseGameLayer;

class $modify (AttemptBaseGameLayer, GJBaseGameLayer)
{
    struct Fields {
        int attemptCount = 0;
    };

    void resetLevelVariables();
};

class StatusNode : public CCNode
{
    public:
        static StatusNode* create() {
            auto ret = new (std::nothrow) StatusNode;
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        ~StatusNode()
        {
            instance = nullptr;
        }

        static inline StatusNode* instance = nullptr;

        static StatusNode* get() { return instance; }

        //NoclipLayer* v;

        bool mods;

        AttemptBaseGameLayer* attPL = nullptr;

        static inline Module* fps = nullptr;
        static inline Module* cheat = nullptr;
        static inline Module* accuracy = nullptr;
        static inline Module* deaths = nullptr;
        static inline Module* replay = nullptr;
        static inline Module* attempt = nullptr;
        static inline Module* message = nullptr;
        static inline Module* session = nullptr;
        static inline Module* cpsM = nullptr;
        static inline Module* bestRun = nullptr;
        static inline Module* clock = nullptr;
        static inline Module* percentage = nullptr;

        static inline Module* noclip = nullptr;

        CCLabelBMFont* title;

        CCMenu* topLeft = nullptr;
        CCMenu* topRight = nullptr;
        CCMenu* bottomLeft = nullptr;
        CCMenu* bottomRight = nullptr;

        static inline Window* window = nullptr;

        std::vector<CCLabelBMFont*> sLabels = {};

        static inline bool hidden = false;

        float _updateInterval = 0.5f;
        float _timeLeft = _updateInterval;
        float _accum;
        int _frames;

        std::vector<float> cps;
        int totalClicks = 0;

        NoclipBaseGameLayer* noclipLayer = nullptr;
        BestPlayLayer* bestRunPlayLayer = nullptr;

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

        std::string formatTime() {
            // Get current time
            std::time_t currentTime = std::time(nullptr);
            
            // Convert to local time
            std::tm* localTime = std::localtime(&currentTime);
            
            // Create a string stream to format the time
            std::ostringstream oss;
            
            if (Client::GetModuleEnabled("status-clock-24h-time"))
                oss << std::put_time(localTime, "%H:%M:%S");
            else
                oss << std::put_time(localTime, "%I:%M:%S %p");

            // Return the formatted time as a string
            return oss.str();
        }


        bool init();

        void updateVis();

        static void postSetup(Window* wnd);

        void reorderSides();
        void reorderPosition();

        void update(float dt);
        void updateCPS(float dt);
};

#endif