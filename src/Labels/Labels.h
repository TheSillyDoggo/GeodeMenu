#ifdef STATUS_TEXTS

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

#include "../Hacks/Noclip.cpp"

using namespace geode::prelude;

class $modify (AttemptPlayLayer, PlayLayer)
{
    struct Fields {
        int attemptCount = 0;
    };

    void resetLevel()
    {
        PlayLayer::resetLevel();

        log::info("attempt");

        m_fields->attemptCount++;
    }
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

        AttemptPlayLayer* attPL = nullptr;

        static inline Module* fps = nullptr;
        static inline Module* cheat = nullptr;
        static inline Module* accuracy = nullptr;
        static inline Module* deaths = nullptr;
        static inline Module* replay = nullptr;
        static inline Module* attempt = nullptr;
        static inline Module* message = nullptr;
        static inline Module* session = nullptr;
        static inline Module* cpsM = nullptr;

        static inline Module* noclip = nullptr;

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

        void update(float dt);
        void updateCPS(float dt);
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        if (getChildByID("status-node"_spr))
            return true;

        auto stn = StatusNode::create();
        stn->attPL = static_cast<AttemptPlayLayer*>(PlayLayer::get());
        this->addChild(stn);
        
        return true;
    }
};

#endif