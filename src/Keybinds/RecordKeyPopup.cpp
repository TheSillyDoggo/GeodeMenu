#include "RecordKeyPopup.hpp"
#include <Geode/ui/GeodeUI.hpp>

bool RecordKeyPopup::init(SEL_MenuHandler obj)
{
    if (!CCLayerColor::init())
        return false;

    this->setOpacity(100);
    this->setKeyboardEnabled(true);
    this->handler = obj;

    CCTouchDispatcher::get()->registerForcePrio(this, 2);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setOpacity(100);
    bg->setContentSize(ccp(245, 70));
    bg->setPosition(CCDirector::get()->getWinSize() / 2);
    this->addChild(bg);

    auto text1 = CCLabelBMFont::create("Waiting for key press", "bigFont.fnt");
    text1->setScale(0.6f);
    text1->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, 10));
    this->addChild(text1);

    auto text2 = CCLabelBMFont::create("Press Escape to cancel", "bigFont.fnt");
    text2->setScale(0.4f);
    text2->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, -10));
    this->addChild(text2);

    if (auto popup = getChildOfType<CCLayer>(CCScene::get(), 0))
    {
        if (auto input = getChildOfType<CCTextInputNode>(popup, 0))
            input->onClickTrackNode(false);
    }

    return true;
}

void RecordKeyPopup::keyDown(enumKeyCodes key)
{
    CCTouchDispatcher::get()->unregisterForcePrio(this);
    CCTouchDispatcher::get()->removeDelegate(this);

    if (CCTouchDispatcher::get()->m_pTargetedHandlers->containsObject(this))
        CCTouchDispatcher::get()->m_pTargetedHandlers->removeObject(this, false);

    if (key == enumKeyCodes::KEY_Escape)
        return this->removeFromParent();

    auto obj = CCNode::create();
    obj->retain();
    obj->setTag(key);
    obj->setUserData(this->getUserData());

    if (handler)
        (this->*handler)(obj);

    obj->release();

    this->removeFromParent();
}

RecordKeyPopup* RecordKeyPopup::create(SEL_MenuHandler obj)
{
    auto pRet = new RecordKeyPopup();

    if (pRet && pRet->init(obj))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}