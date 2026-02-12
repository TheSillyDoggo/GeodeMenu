#include "ImageNode.hpp"

void ImageNode::labelConfigUpdated()
{
    if (sprite)
    {
        sprite->removeFromParent();
        sprite = nullptr;
    }

    sprite = CCSprite::create(config.imageLocation.c_str());

    if (!sprite)
    {
        sprite = CCSprite::create("sog.png"_spr);
    }

    if (!sprite)
        return;

    sprite->setAnchorPoint(ccp(0, 0));
    this->addChild(sprite);
}

void ImageNode::update(float dt)
{
    if (sprite)
    {
        sprite->setScale(std::min<float>(75.0f / sprite->getContentHeight(), 1));
        this->setContentSize(sprite->getScaledContentSize());
    }
    else
    {
        this->setContentSize(ccp(0, 0));
    }
}