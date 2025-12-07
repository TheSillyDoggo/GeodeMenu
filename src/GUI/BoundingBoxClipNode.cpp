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

    GLint viewporta[4];
    glGetIntegerv(GL_VIEWPORT, viewporta);

    log::error("balls: 1: {}, 2: {}, 3: {}, 4: {}", viewporta[0], viewporta[1], viewporta[2], viewporta[3]);
    log::error("balls: 5: {}, 6: {}, 7: {}, 8: {}", CCEGLView::get()->m_obViewPortRect.origin.x, CCEGLView::get()->m_obViewPortRect.origin.y, CCEGLView::get()->m_obViewPortRect.size.width, CCEGLView::get()->m_obViewPortRect.size.height);
}

class $modify (CCEGLView)
{
    virtual void setScissorInPoints(float x , float y , float w , float h)
    {        
        #ifdef GEODE_IS_WINDOWS

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float zoomFactor = m_fFrameZoomFactor;

        float multWidth = viewport[2] / (m_obScreenSize.width * zoomFactor);
        float multHeight = viewport[3] / (m_obScreenSize.height * zoomFactor);

        x *= multWidth;
        y *= multHeight;
        w *= multWidth;
        h *= multHeight;

        return CCEGLView::setScissorInPoints(x, y, w, h);

        #endif

        GLint viewporta[4];
        glGetIntegerv(GL_VIEWPORT, viewporta);

        log::error("1: {}, 2: {}, 3: {}, 4: {}", viewporta[0], viewporta[1], viewporta[2], viewporta[3]);
        log::error("5: {}, 6: {}, 7: {}, 8: {}", m_obViewPortRect.origin.x, m_obViewPortRect.origin.y, m_obViewPortRect.size.width, m_obViewPortRect.size.height);

        #ifdef GEODE_IS_ANDROID

        return CCEGLView::setScissorInPoints(x, y, w, h);

        #endif

        CCEGLView::setScissorInPoints(x, y, w, h);
    }
};