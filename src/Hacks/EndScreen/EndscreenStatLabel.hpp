#pragma once

#include <Geode/Geode.hpp>
#include <AdvancedLabel/AdvLabelBMFont.hpp>

enum class EndscreenStatType
{
    Attempts,
    Time,
    Jumps,
    Points,
    NoclipDeaths,
    NoclipAccuracy,
};

class EndscreenStatLabel : public cocos2d::CCMenu
{
    protected:
        AdvLabelBMFont* label;
        EndscreenStatType type;
        std::function<void()> callback = nullptr;

        std::string getString(GJBaseGameLayer* layer);
        bool isClickable();
        void onCallback(CCObject* sender);

    public:
        static EndscreenStatLabel* create(EndscreenStatType type);

        void setCallback(std::function<void()> callback);

        bool init(EndscreenStatType type);
};