#include "ScrollLayer.hpp"

using namespace geode::prelude;
using namespace qolmod;

qolmod::ScrollLayer* qolmod::ScrollLayer::create(cocos2d::CCSize const& size, bool scrollWheelEnabled, bool vertical)
{
    auto ret = new qolmod::ScrollLayer(CCRectMake(0, 0, size.width, size.height), scrollWheelEnabled, vertical);
    ret->autorelease();
    return ret;
}

qolmod::ScrollLayer::ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical) : geode::ScrollLayer::ScrollLayer(rect, scrollWheelEnabled, vertical)
{
    clipStencil = CCLayerColor::create(ccc4(255, 255, 255, 255), rect.size.width, rect.size.height);
    clipStencil->ignoreAnchorPointForPosition(false);
    clipStencil->setAnchorPoint(ccp(0, 0));

    clipStencil->setVisible(false);
    this->addChild(clipStencil);

    m_peekLimitTop = 15;
    m_peekLimitBottom = 15;

    this->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());
    this->setUserObject("MouseDispatcherExt"_spr, CCNode::create());
}

void qolmod::ScrollLayer::visit()
{   
    if (!this->isVisible())
        return;

    clipStencil->setContentSize(getContentSize());

    GLboolean stencilEnabled;
    glGetBooleanv(GL_STENCIL_TEST, &stencilEnabled);

    GLint stencilFunc, stencilRef, stencilMask;
    glGetIntegerv(GL_STENCIL_FUNC, &stencilFunc);
    glGetIntegerv(GL_STENCIL_REF, &stencilRef);
    glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencilMask);

    GLint stencilFail, stencilPassDepthFail, stencilPassDepthPass;
    glGetIntegerv(GL_STENCIL_FAIL, &stencilFail);
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &stencilPassDepthFail);
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &stencilPassDepthPass);

    glEnable(GL_STENCIL_TEST);

    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    kmGLPushMatrix();
    this->transform();
    clipStencil->draw();
    kmGLPopMatrix();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    CCNode::visit();

    glStencilFunc(stencilFunc, stencilRef, stencilMask);
    glStencilOp(stencilFail, stencilPassDepthFail, stencilPassDepthPass);

    if (!stencilEnabled)
        glDisable(GL_STENCIL_TEST);
}