//#define STATUS_TEXTS

#ifdef STATUS_TEXTS

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

#include "../Hacks/Noclip.cpp"

using namespace geode::prelude;

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

        //NoclipLayer* v;

        bool mods;

        CCLabelBMFont* attemptText = nullptr;

        static inline Module* fps = nullptr;
        static inline Module* cheat = nullptr;
        static inline Module* accuracy = nullptr;
        static inline Module* deaths = nullptr;
        static inline Module* replay = nullptr;
        static inline Module* attempt = nullptr;
        static inline Module* message = nullptr;
        static inline Module* session = nullptr;

        static inline Module* noclip = nullptr;

        CCMenu* topLeft = nullptr;
        CCMenu* topRight = nullptr;
        CCMenu* bottomLeft = nullptr;
        CCMenu* bottomRight = nullptr;

        static inline Window* window = nullptr;

        std::vector<CCLabelBMFont*> sLabels = {};

        static inline bool hidden = false;

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
};

class $modify (PlayLayer)
{
    StatusNode* stn;

    virtual void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        //m_fields->stn->attemptText = m_attemptLabel;

        return;
        
        if (m_fields->stn && !m_fields->stn->attemptText)
        {
            CCLayer* mainLayer = this->m_objectLayer;
            log::info("mainLayer: {}", mainLayer);
            auto lbl = getChildOfType<CCLabelBMFont>(mainLayer, 0);
            log::info("label: {}", lbl);

            auto stn2 = this->m_fields->stn;
            log::info("status node: {}", stn2);
            stn2->attemptText = lbl;
            log::info("sex");
        }
    }

    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        if (this->getChildByID("status-text-menu"_spr))
            return true;

        CCLayer* mainLayer = nullptr;

        if (!mainLayer)
        {
            if (auto mainNode = getChildOfType<CCNode>(this, 1))
            {
                if (auto l = getChildOfType<CCLayer>(mainNode, 0))
                {
                    mainLayer = l;
                }
            }
        }

        auto stn = StatusNode::create();
        stn->attemptText = m_attemptLabel;
        log::info("attemptText: {}", stn->attemptText);

        auto menu = CCMenu::create();
        menu->setID("status-text-menu"_spr);
        menu->setPosition(ccp(0, 0));
        menu->setContentSize(CCDirector::get()->getWinSize());
        menu->setAnchorPoint(ccp(0, 0));
        menu->ignoreAnchorPointForPosition(false);

        this->addChild(stn);

        this->addChild(menu, 69420);

        m_fields->stn = stn;
        
        return true;
    }
};

#endif