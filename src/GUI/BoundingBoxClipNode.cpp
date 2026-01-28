#include "BoundingBoxClipNode.hpp"
#include <Geode/modify/CCEGLView.hpp>

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
    auto parent = this->getParent();
	auto bounding_box = this->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

    node->setPosition(-min);

    GameToolbox::preVisitWithClippingRect(node, CCRectMake(min.x, min.y, max.x - min.x, max.y - min.y));

    CCNode::visit();

    glDisable(0xc11);
}