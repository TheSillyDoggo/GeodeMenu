#pragma once

#include <Geode/Geode.hpp>
#include "NotificationNode.hpp"

class NotificationManager : public cocos2d::CCNode
{
    public:
        static NotificationManager* get();

        void notifyToast(std::string toastStr, float time = -1);
        void removeNotification(NotificationNode* node);

        virtual void update(float dt);
};