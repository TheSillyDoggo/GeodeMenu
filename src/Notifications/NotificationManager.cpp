#include "NotificationManager.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../GUI/EasyBG.hpp"
#include "Modules/Modules.hpp"
#include <Speedhack.hpp>

using namespace geode::prelude;

NotificationManager* NotificationManager::get()
{
    static NotificationManager* instance = nullptr;

    if (!instance)
    {
        instance = new NotificationManager();
        instance->scheduleUpdate();
        instance->onEnter();
    }

    return instance;
}

void NotificationManager::notifyToast(std::string toastStr, float time)
{
    if (time == -1)
        time = NotificationsDuration::get()->getStringFloat();

    auto n = NotificationNode::create(toastStr, time);
    n->setPosition(ccp(5, 100));
    this->addChild(n);
}

void NotificationManager::removeNotification(NotificationNode* node)
{
    node->removeFromParent();
    delete node;
}

void NotificationManager::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    bool right = NotificationsRight::get()->getRealEnabled();

    float y = 0;
    int i = 0;
    for (auto node : CCArrayExt<NotificationNode*>(getChildren()))
    {
        node->setPosition(ccp(right ? CCDirector::get()->getWinSize().width - 5 : 5, CCDirector::get()->getWinSize().height - 5 + y));
        node->setAnchorPoint(ccp(right ? 1 : 0, 1));

        if (i != getChildrenCount() - 1)
            y += node->getScaledContentHeight() + 5;

        i++;
    }

    if (getPositionY() < -y)
        setPositionY(-y);

    this->setPositionY(std::lerp<double>(getPositionY(), -y, 10 * dt));
}