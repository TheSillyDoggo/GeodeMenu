#include "NotificationNode.hpp"
#include "../Utils/ColourUtils.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../GUI/EasyBG.hpp"
#include "NotificationManager.hpp"
#include <BlurAPI.hpp>
#include "Modules/Modules.hpp"
#include "../Utils/RealtimeAction.hpp"
#include <Speedhack.hpp>

using namespace geode::prelude;

NotificationNode* NotificationNode::create(std::string msg, float time)
{
    auto pRet = new NotificationNode();

    if (pRet && pRet->init(msg, time))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool NotificationNode::init(std::string msg, float time)
{
    if (!CCNode::init())
        return false;

    this->scheduleUpdate();
    this->setAnchorPoint(ccp(0, 1));
    this->setScale(0.3f);
    this->time = time;

    this->node = CCNode::create();

    auto lbl = AdvLabelBMFont::createWithString(msg, "bigFont.fnt");
    auto bg = EasyBG::create();
    bg->setTargettingNode(lbl);
    bg->setTargettingOffset(ccp(15, 15 + 5));
    bg->setAnchorPoint(ccp(0, 0));

    if (NotificationsBlur::get()->getRealEnabled())
        BlurAPI::addBlur(bg);

    bg->visit();
    lbl->setPosition(bg->getContentSize() / 2 + ccp(0, 5.0f / 2));
    this->setContentSize(bg->getContentSize());

    progress = CCLayerColor::create(ccc4(0, 0, 0, 255));
    progress->setContentSize(ccp(bg->getContentWidth(), 5));
    progress->setAnchorPoint(ccp(0, 0));
    progress->setPosition(ccp(0, 0));
    progress->ignoreAnchorPointForPosition(false);

    node->setPositionX(-bg->getContentWidth() - 20);
    node->runAction(RealtimeAction::create(CCSequence::create(
        CCEaseInOut::create(CCMoveTo::create(0.1f, ccp(0, 0)), 2),
        CCDelayTime::create(time - 0.1f * 2),
        CCEaseInOut::create(CCMoveTo::create(0.1f, ccp(-bg->getContentWidth() - 20, 0)), 2),
        CCDelayTime::create(0.1f),
        CCCallFunc::create(this, callfunc_selector(NotificationNode::notificationEnded)),
    nullptr)));

    node->addChild(bg);
    node->addChild(lbl);
    node->addChild(progress);
    this->addChild(node);
    return true;
}

void NotificationNode::notificationEnded(CCObject* sender)
{
    NotificationManager::get()->removeNotification(this);
}

void NotificationNode::update(float dt)
{
    prog += Speedhack::get()->getRealDeltaTime();
    progress->setColor(ColourUtils::get()->getPastel("notification-colour"));
    progress->setScaleX(std::clamp<float>(prog / time, 0, 1));
}

void NotificationNode::visit()
{
    if (!NotificationsRight::get()->getRealEnabled())
        return CCNode::visit();

    node->setPositionX(-node->getPositionX());

    CCNode::visit();

    node->setPositionX(-node->getPositionX());
}