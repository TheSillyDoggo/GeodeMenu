#include "CategoryNode.hpp"

CategoryNode* CategoryNode::create()
{
    auto pRet = new CategoryNode();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void CategoryNode::addModule(Module* module)
{

}

bool CategoryNode::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(1, 0.5f));
    this->setContentSize(ccp(300, 280 - 10 * 2));

    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setContentSize(this->getContentSize() / 0.5f);
    bg->setScale(0.5f);
    bg->setColor(ccColor3B(0, 0, 0));
    bg->setOpacity(100);

    this->addChildAtPosition(bg, Anchor::Center);
    return true;
}