#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <NotificationManager.hpp>

bool isPlayback = true;
double timestampStart = 0;
double timestampClick = 0;

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void resetLevelVariables()
    {
        GJBaseGameLayer::resetLevelVariables();

        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);

        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);

        // log::warn("recreated: {}", (double)counter.QuadPart / (double)freq.QuadPart);
        // log::warn("m_ts: {}", m_timestamp);

        // timestampStart = (double)counter.QuadPart / (double)freq.QuadPart;
        timestampStart = m_timestamp;
    }
};

class $modify (UILayer)
{
    void handleKeypress(cocos2d::enumKeyCodes key, bool down, double timestamp)
    {
        UILayer::handleKeypress(key, down, timestamp);

        if (!isPlayback)
            timestampClick = this->m_gameLayer->m_timestamp - timestampStart;

        // log::info("clicked: {}, m_timeStamp: {}", timestamp, this->m_gameLayer->m_timestamp);
    }
};

class $modify (PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* player, GameObject* object)
    {
        PlayLayer::destroyPlayer(player, object);

        // if (object != m_anticheatSpike)
            // log::error("died: {}", m_timestamp - timestampStart);
    }
};