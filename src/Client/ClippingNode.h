#pragma once

#include <Geode/Geode.hpp>

//definetly not ai generated

using namespace geode::prelude;

class ClippingNode : public CCMenu {
private:
    CCRect m_clipRect;
    
public:
    static ClippingNode* create(const CCRect& clipRect) {
        ClippingNode* node = new ClippingNode();
        if (node && node->initWithClipRect(clipRect)) {
            node->autorelease();
            return node;
        }
        CC_SAFE_DELETE(node);
        return NULL;
    }
    
    bool initWithClipRect(const CCRect& clipRect) {
        if (!CCMenu::init()) {
            return false;
        }
        m_clipRect = clipRect;
        return true;
    }

    void setClipRect(const CCRect& clipRect)
    {
        m_clipRect = clipRect;
    }
    
    virtual void visit() {
        if (!m_clipRect.equals(CCRectZero)) {
            glEnable(GL_SCISSOR_TEST);
            CCEGLView::sharedOpenGLView()->setScissorInPoints(m_clipRect.origin.x, m_clipRect.origin.y, m_clipRect.size.width, m_clipRect.size.height);
            CCMenu::visit();
            glDisable(GL_SCISSOR_TEST);
        } else {
            CCMenu::visit();
        }
    }
};