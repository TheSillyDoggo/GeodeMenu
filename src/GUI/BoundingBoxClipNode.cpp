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

#ifdef GEODE_IS_WINDOWS

class $modify (CCEGLView)
{
    virtual void setScissorInPoints(float x , float y , float w , float h)
    {

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float zoomFactor = m_fFrameZoomFactor;

        float multWidth = viewport[2] / (m_obScreenSize.width * zoomFactor);
        float multHeight = viewport[3] / (m_obScreenSize.height * zoomFactor);

        x *= multWidth;
        y *= multHeight;
        w *= multWidth;
        h *= multHeight;

        CCEGLView::setScissorInPoints(x, y, w, h);
    }
};

#endif