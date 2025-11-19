#include "KeycodeListenerLayer.hpp"
#include "KeycodeNode.hpp"

KeycodeListenerLayer* KeycodeListenerLayer::create(int currentBind, std::function<void(int)> onSucceed)
{
    auto pRet = new KeycodeListenerLayer();
    pRet->currentBind = currentBind;
    pRet->onSucceed = onSucceed;

    pRet->initAnchored(0, 0);
    return pRet;
}

void KeycodeListenerLayer::keyDown(cocos2d::enumKeyCodes key)
{
    if (key == enumKeyCodes::KEY_Escape)
    {
        this->removeFromParent();
        return;
    }

    if (key == enumKeyCodes::KEY_Backspace)
    {
        if (onSucceed)
            onSucceed(-1);

        this->removeFromParent();
        return;
    }

    if (onSucceed)
        onSucceed(static_cast<int>(key));

    this->removeFromParent();
    return;
}

void KeycodeListenerLayer::keyBackClicked()
{
    keyDown(enumKeyCodes::KEY_Escape);
}

void KeycodeListenerLayer::onCloseBtn(CCObject* sender)
{
    keyDown(enumKeyCodes::KEY_Escape);
}

bool KeycodeListenerLayer::setup()
{
    this->m_mainLayer->setVisible(false);
    this->setAnchorPoint(ccp(0, 0));

    this->stopAllActions();
    this->runAction(CCFadeTo::create(0.25f, 200));

    auto title = CCLabelBMFont::create("Waiting for key press", "bigFont.fnt");
    title->setScale(0.7f);

    auto cancelContainer = CCNode::create();
    cancelContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    cancelContainer->setContentWidth(80085);
    cancelContainer->setScale(0.475);

    auto cancel1 = CCLabelBMFont::create("Press", "bigFont.fnt");
    auto cancel2 = KeycodeNode::create(27);
    cancel2->setContentWidth(55);
    cancel2->setScale(1.3f);
    cancel2->setAnchorPoint(ccp(0.5f, 0.8f));
    cancel2->setLayoutOptions(AxisLayoutOptions::create()->setCrossAxisAlignment(AxisAlignment::Start));
    auto cancel3 = CCLabelBMFont::create("to cancel", "bigFont.fnt");

    cancelContainer->addChild(cancel1);
    cancelContainer->addChild(cancel2);
    cancelContainer->addChild(cancel3);


    auto deleteContainer = CCNode::create();
    deleteContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    deleteContainer->setContentWidth(80085);
    deleteContainer->setScale(0.475);

    auto delete1 = CCLabelBMFont::create("Press", "bigFont.fnt");
    auto delete2 = KeycodeNode::create(8);
    delete2->setContentWidth(55);
    delete2->setScale(1.3f);
    delete2->setAnchorPoint(ccp(0.5f, 0.8f));
    delete2->setLayoutOptions(AxisLayoutOptions::create()->setCrossAxisAlignment(AxisAlignment::Start));
    auto delete3 = CCLabelBMFont::create("to delete bind", "bigFont.fnt");

    deleteContainer->addChild(delete1);
    deleteContainer->addChild(delete2);
    deleteContainer->addChild(delete3);

    
    auto currentContainer = CCNode::create();
    currentContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    currentContainer->setContentWidth(80085);
    currentContainer->setScale(0.525f);

    auto current1 = CCLabelBMFont::create("Current key:", "bigFont.fnt");
    auto current2 = KeycodeNode::create(this->currentBind);
    current2->setScale(1.5f);
    current2->setAnchorPoint(ccp(0.5f, 0.8f));
    current2->setLayoutOptions(AxisLayoutOptions::create()->setCrossAxisAlignment(AxisAlignment::Start));

    currentContainer->addChild(current1);
    currentContainer->addChild(current2);


    cancelContainer->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setGap(10));
    deleteContainer->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setGap(10));
    currentContainer->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setGap(10));

    auto menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setScale(0.8f);
    auto closeBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), this, menu_selector(KeycodeListenerLayer::onCloseBtn));
    menu->addChild(closeBtn);

    this->addChildAtPosition(title, Anchor::Top, ccp(0, -25));
    this->addChildAtPosition(cancelContainer, Anchor::Center, ccp(0, 10));
    this->addChildAtPosition(deleteContainer, Anchor::Center, ccp(0, -10));
    this->addChildAtPosition(currentContainer, Anchor::Center, ccp(0, 50));
    this->addChildAtPosition(menu, Anchor::TopLeft, ccp(25, -25));
    return true;
}