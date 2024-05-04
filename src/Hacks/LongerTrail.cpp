/*#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    bool initWithFade(float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture)
    {
        CCTexture2D* oldTexture = texture;

        // Double the width of the texture
        int oldWidth = oldTexture->getPixelsWide();
        int oldHeight = oldTexture->getPixelsHigh();
        int newWidth = oldWidth * 2; // Double the width
        CCRenderTexture* renderTexture = CCRenderTexture::create(newWidth, oldHeight);
        renderTexture->begin();
        // Draw the old texture at the center of the new texture
        CCSprite* sprite = CCSprite::createWithTexture(oldTexture);
        sprite->setPosition(ccp(newWidth / 2, oldHeight / 2));
        sprite->visit();
        renderTexture->end();
        CCTexture2D* newTexture = renderTexture->getSprite()->getTexture();

        return CCMotionStreak::initWithFade(fade, minSeg, stroke, color, newTexture);
    }
};*/