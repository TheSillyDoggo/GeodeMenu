#include "CCNodeWithShader.hpp"

using namespace geode::prelude;
using namespace qolmod;

bool CCNodeWithShader::init()
{
    if (!CCNode::init())
        return false;

    if (rt)
        CC_SAFE_DELETE(rt);

    rt = CCRenderTexture::create(offset.width, offset.height, kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);
    #if GEODE_COMP_GD_VERSION >= 22081
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    #endif

    #ifdef GEODE_IS_ANDROID
    if (!listened)
    {
        listened = true;

        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
            callfuncO_selector(CCNodeWithShader::listenBackToForeground),
            EVENT_COME_TO_FOREGROUND,
            nullptr);
    }
    #endif

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

void CCNodeWithShader::listenBackToForeground(CCObject* sender)
{
    init();
}

CCNodeWithShader::~CCNodeWithShader()
{
    #ifdef GEODE_IS_ANDROID
    if (listened)
        CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    #endif
}