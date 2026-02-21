#include "CCNodeWithShader.hpp"

using namespace geode::prelude;

bool CCNodeWithShader::init()
{
    if (!CCNode::init())
        return false;

    rt = CCRenderTexture::create(offset.width, offset.height, kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();

    return true;
}

void CCNodeWithShader::visit()
{
    rt->beginWithClear(0, 0, 0, 0);

    kmGLPushMatrix();
    kmGLTranslatef(offset.width / 2, offset.height / 2, 0);

    for (auto child : CCArrayExt<CCNode*>(getChildren()))
    {
        child->visit();
    }
    

    kmGLPopMatrix();
    rt->end();

    kmGLPushMatrix();
    CCNode::transform();

    rt->getSprite()->setColor(getColor());
    rt->getSprite()->setOpacity(getOpacity());
    rt->getSprite()->setPosition(ccp(0, 0));
    rt->visit();

    kmGLPopMatrix();
}