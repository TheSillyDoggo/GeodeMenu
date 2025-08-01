#include "BoundingBoxClipNode.hpp"

bool BoundingBoxClipNode::init()
{
    if (!CCNode::init())
        return false;

    node = CCNode::create();
    this->addChild(node);

    return true;
}

void BoundingBoxClipNode::visit()
{
    node->setPosition(-ccp(boundingBox().getMinX(), boundingBox().getMinY()));

    GameToolbox::preVisitWithClippingRect(node, boundingBox());

    CCNode::visit();

    glDisable(0xc11);
}