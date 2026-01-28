#pragma once

#include <Geode/Geode.hpp>

class NotificationNode : public cocos2d::CCNode
{
    protected:
        cocos2d::CCLayerColor* progress = nullptr;
        float prog = 0;
        float time = 0;
        cocos2d::CCNode* node;

    public:
        static NotificationNode* create(std::string msg, float time);

        void notificationEnded(CCObject* sender);
        bool init(std::string msg, float time);

        virtual void update(float dt);
        virtual void visit();
};