#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

struct Notif
{
    std::string text = "";
    bool playSound = true;
    float duration = 5;
};

class NotifLayer : public CCLayer
{
    protected:
        std::vector<Notif> queue = {};
        Notif notif;
        CCLayerColor* bg;
        CCLabelBMFont* label;
        std::function<void()> onFinish = nullptr;
        static inline NotifLayer* instance = nullptr;

        ~NotifLayer();

    public:
        static NotifLayer* createAndNotify(Notif notif);
        static NotifLayer* create(Notif notif);
        static void closeCurrent();

        void notify();
        void onTimeUp(float dt);
        void addToQueue(Notif notif);
        void setOnFinish(std::function<void()> func);

        bool init(Notif notif);
};