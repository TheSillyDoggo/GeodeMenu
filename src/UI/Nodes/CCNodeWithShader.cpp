#include "CCNodeWithShader.hpp"

using namespace geode::prelude;

bool CCNodeWithShader::init()
{
    if (!CCNode::init())
        return false;

    rt = CCRenderTexture::create(CCDirector::get()->getWinSize().width, CCDirector::get()->getWinSize().height, kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();

    return true;
}

void CCNodeWithShader::visit()
{
    rt->beginWithClear(0, 0, 0, 0);
    CCNode::visit();
    rt->end();

    rt->getSprite()->setColor(getColor());
    rt->getSprite()->setOpacity(getOpacity());
    rt->getSprite()->setPosition(CCDirector::get()->getWinSize() / 2);
    rt->visit();
}