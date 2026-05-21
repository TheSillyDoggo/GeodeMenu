#include "LayerColor.hpp"

using namespace geode::prelude;
using namespace qolmod;

LayerColor* LayerColor::create()
{
    LayerColor* pRet = new LayerColor();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

LayerColor* LayerColor::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    LayerColor * pLayer = new LayerColor();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

LayerColor* LayerColor::create(const ccColor4B& color)
{
    LayerColor * pLayer = new LayerColor();
    if(pLayer && pLayer->initWithColor(color))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

void LayerColor::draw()
{
    CC_NODE_DRAW_SETUP();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);

    ccGLBlendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    /*glBlendFuncSeparate(
        GL_ONE_MINUS_DST_COLOR,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );*/

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}