#include "EditSafeZonePopup.hpp"
#include "../Client/Windows/Labels.hpp"

#define CORNER_SIZE 1.5f

void EditSafeZonePopup::customSetup()
{
    this->stopAllActions();
    this->runAction(CCFadeTo::create(0.25f, 100));
    this->scheduleUpdate();
    bg->setVisible(false);

    CCLayerColor* node;
    CCLabelBMFont* label;

    bottomLeft = CCNode::create();
    
    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE, CORNER_SIZE * 8);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(0, CORNER_SIZE));
    bottomLeft->addChild(node);

    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE * 8, CORNER_SIZE);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(CORNER_SIZE, 0));
    bottomLeft->addChild(node);

    bottomRight = CCNode::create();
    bottomRight->setRotation(-90);
    
    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE, CORNER_SIZE * 8);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(0, CORNER_SIZE));
    bottomRight->addChild(node);

    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE * 8, CORNER_SIZE);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(CORNER_SIZE, 0));
    bottomRight->addChild(node);

    topRight = CCNode::create();
    topRight->setRotation(-180);
    
    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE, CORNER_SIZE * 8);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(0, CORNER_SIZE));
    topRight->addChild(node);

    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE * 8, CORNER_SIZE);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(CORNER_SIZE, 0));
    topRight->addChild(node);

    topLeft = CCNode::create();
    topLeft->setRotation(-270);
    
    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE, CORNER_SIZE * 8);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(0, CORNER_SIZE));
    topLeft->addChild(node);

    node = CCLayerColor::create(ccc4(150, 150, 150, 255), CORNER_SIZE * 8, CORNER_SIZE);
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(CORNER_SIZE, 0));
    topLeft->addChild(node);

    label = CCLabelBMFont::create("Test Label", "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(0, 0));
    bottomLeft->addChild(label);

    label = CCLabelBMFont::create("Test Label", "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(1, 0));
    label->setRotation(-bottomRight->getRotation());
    bottomRight->addChild(label);

    label = CCLabelBMFont::create("Test Label", "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(1, 1));
    label->setRotation(-topRight->getRotation());
    topRight->addChild(label);

    label = CCLabelBMFont::create("Test Label", "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(0, 1));
    label->setRotation(-topLeft->getRotation());
    topLeft->addChild(label);

    this->addChild(bottomLeft);
    this->addChild(bottomRight);
    this->addChild(topLeft);
    this->addChild(topRight);
}

void EditSafeZonePopup::update(float dt)
{
    bottomLeft->setPosition(Labels::get()->safeZone.origin);
    bottomRight->setPosition(ccp(CCDirector::get()->getWinSize().width - Labels::get()->safeZone.size.width, Labels::get()->safeZone.origin.y));
    topLeft->setPosition(ccp(Labels::get()->safeZone.origin.x, CCDirector::get()->getWinSize().height - Labels::get()->safeZone.size.height));
    topRight->setPosition(CCDirector::get()->getWinSize() - Labels::get()->safeZone.size);
}

EditSafeZonePopup* EditSafeZonePopup::create()
{
    auto pRet = new EditSafeZonePopup();

    if (pRet && pRet->initWithSizeAndName(ccp(0, CCDirector::get()->getWinSize().height - 0), "", true, false, true))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

EditSafeZonePopup* EditSafeZonePopup::addToScene()
{
    auto pRet = EditSafeZonePopup::create();

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}