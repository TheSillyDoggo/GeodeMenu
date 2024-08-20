#include "RecordKeyStruct.hpp"
#include <Geode/ui/GeodeUI.hpp>

bool RecordKeyStruct::init(geode::utils::MiniFunction<void(KeyStruct)> obj)
{
    if (!CCLayerColor::init())
        return false;

    this->setOpacity(100);
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->scheduleUpdate();
    this->callback = obj;

    CCTouchDispatcher::get()->registerForcePrio(this, 2);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);

    auto bg = CCScale9Sprite::create("square02_small.png");
    bg->setOpacity(200);
    bg->setContentSize(ccp(245, 90));
    bg->setPosition(CCDirector::get()->getWinSize() / 2);
    this->addChild(bg);

    auto text1 = CCLabelBMFont::create("Waiting for key press", "bigFont.fnt");
    text1->setScale(0.6f);
    text1->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, 30));
    this->addChild(text1);

    auto text2 = CCLabelBMFont::create("Press Escape to cancel", "bigFont.fnt");
    text2->setScale(0.4f);
    text2->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, 10));
    this->addChild(text2);

    auto node = CCNode::create();
    node->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, -30));
    node->setLayout(AxisLayout::create()->setAutoScale(false)->setGap(10));
    node->setContentWidth(42069); // meow meow mrrp mrrow
    node->setAnchorPoint(ccp(0.5f, 0.5f));

    shift = CCLabelBMFont::create("Shift", "bigFont.fnt");
    shift->setScale(0.5f);
    ctrl = CCLabelBMFont::create("Ctrl", "bigFont.fnt");
    ctrl->setScale(0.5f);
    alt = CCLabelBMFont::create("Alt", "bigFont.fnt");
    alt->setScale(0.5f);
    cmd = CCLabelBMFont::create("Command", "bigFont.fnt");
    cmd->setScale(0.5f);

    if (auto popup = getChildOfType<CCLayer>(CCScene::get(), 0))
    {
        if (auto input = getChildOfType<CCTextInputNode>(popup, 0))
            input->onClickTrackNode(false);
    }

    node->addChild(shift);
    node->addChild(ctrl);
    node->addChild(alt);
    node->addChild(cmd);
    node->updateLayout();

    this->addChild(node);
    return true;
}

void RecordKeyStruct::keyDown(enumKeyCodes key)
{
    if (key == enumKeyCodes::KEY_Unknown)
        return;

    if (key == enumKeyCodes::KEY_Shift)
        return;

    if (key == enumKeyCodes::KEY_LeftShift)
        return;

    if (key == enumKeyCodes::KEY_RightShift)
        return;

    if (key == enumKeyCodes::KEY_Alt)
        return;

    if (key == enumKeyCodes::KEY_Control)
        return;

    if (key == enumKeyCodes::KEY_Home)
        return;
    
    CCTouchDispatcher::get()->unregisterForcePrio(this);
    CCTouchDispatcher::get()->removeDelegate(this);

    if (CCTouchDispatcher::get()->m_pTargetedHandlers->containsObject(this))
        CCTouchDispatcher::get()->m_pTargetedHandlers->removeObject(this, false);

    if (key == enumKeyCodes::KEY_Escape)
        return this->removeFromParent();

    KeyStruct str;

    str.alt = CCKeyboardDispatcher::get()->getAltKeyPressed();
    str.command = CCKeyboardDispatcher::get()->getCommandKeyPressed();
    str.control = CCKeyboardDispatcher::get()->getControlKeyPressed();
    str.shift = CCKeyboardDispatcher::get()->getShiftKeyPressed();
    str.key = key;

    callback(str);

    this->removeFromParent();
}

void RecordKeyStruct::keyBackClicked()
{
    this->keyDown(enumKeyCodes::KEY_Escape);
}

void RecordKeyStruct::update(float dt)
{
    shift->setColor(CCKeyboardDispatcher::get()->getShiftKeyPressed() ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
    ctrl->setColor(CCKeyboardDispatcher::get()->getControlKeyPressed() ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
    alt->setColor(CCKeyboardDispatcher::get()->getAltKeyPressed() ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
    cmd->setColor(CCKeyboardDispatcher::get()->getCommandKeyPressed() ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
}

RecordKeyStruct* RecordKeyStruct::create(geode::utils::MiniFunction<void(KeyStruct)> obj)
{
    auto pRet = new RecordKeyStruct();

    if (pRet && pRet->init(obj))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}